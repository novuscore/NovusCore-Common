/*
 * Secure Remote Password 6a implementation
 * Copyright (c) 2010 Tom Cocagne. All rights reserved.
 * https://github.com/cocagne/csrp
 *
 * The MIT License (MIT)
 *
 * Copyright (c) 2013 Tom Cocagne
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies
 * of the Software, and to permit persons to whom the Software is furnished to do
 * so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 *
 */
#ifdef _WIN32
# include <windows.h>
#else
# include <sys/time.h>
#endif
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <openssl/bn.h>
#include <openssl/sha.h>
#include <openssl/crypto.h>
#include <openssl/rand.h>
#include "srp.h"
#include "../Math/sha256.h"

NGConstant* SRPUtils::_ng = nullptr;
bool SRPUtils::_randomInitialized = false;
NGConstant* SRPUtils::GetNG(const SRP_NGType ngType /* = SRP_NGType::SRP_NG_2048 */)
{
    if (!_ng)
    {
        _ng = new NGConstant();

        BN_hex2bn(&_ng->n, ngConstants[ngType].n.c_str());
        BN_hex2bn(&_ng->g, ngConstants[ngType].g.c_str());
    }

    return _ng;
}
void SRPUtils::RandomInit()
{
    if (_randomInitialized)
        return;

    _randomInitialized = 1;

    // TODO: Replace with Crypto Safe Random
    srand(static_cast<u32>(time(nullptr)));
    u8 buff[64] = { 0 };
    for (i32 i = 0; i < 64; i++)
    {
        buff[i] = rand();
    }

#ifndef WIN32
    FILE* fp = 0;
    fp = fopen("/dev/urandom", "r");
    if (fp)
    {
        fread(buff, sizeof(buff), 1, fp);
        fclose(fp);
        g_initialized = 1;
    }
#endif

    RAND_seed(buff, 64);
}

// These function is using the rfc5054 standard
static void HashNumber(const BIGNUM* n, u8* dest)
{
    i32 nbytes = BN_num_bytes(n);
    u8* bin = (u8*)malloc(nbytes);
    if (!bin)
        return;
    BN_bn2bin(n, bin);

    Sha256 hasher;
    hasher.Hash(bin, nbytes, dest);
    free(bin);
}
static BIGNUM* HashNumbers(const BIGNUM* n, const BIGNUM* n1, const BIGNUM* n2)
{
    u8  buff[SHA256_DIGEST_LENGTH];
    i32 len_N = BN_num_bytes(n);
    i32 len_n1 = BN_num_bytes(n1);
    i32 len_n2 = BN_num_bytes(n2);
    i32 nbytes = len_N * 2;
    u8* bin = (u8*)malloc(nbytes);
    if (!bin)
        return 0;

    if (len_n1 > len_N || len_n2 > len_N)
        return 0;

    memset(bin, 0, nbytes);
    BN_bn2bin(n1, bin + (len_N - len_n1));
    BN_bn2bin(n2, bin + (len_N + len_N - len_n2));

    Sha256 hasher;
    hasher.Hash(bin, nbytes, buff);
    free(bin);
    return BN_bin2bn(buff, SHA256_DIGEST_LENGTH, NULL);
}
static BIGNUM* HashNumberString(const BIGNUM* n, const u8* bytes, const size_t len_bytes)
{
    u8  buff[SHA256_DIGEST_LENGTH];
    size_t len_n = static_cast<size_t>(BN_num_bytes(n));
    size_t nbytes = len_n + len_bytes;
    u8* bin = (u8*)malloc(nbytes);
    if (!bin)
        return 0;
    BN_bn2bin(n, bin);
    memcpy(bin + len_n, bytes, len_bytes);

    Sha256 hasher;
    hasher.Hash(bin, nbytes, buff);
    free(bin);
    return BN_bin2bn(buff, SHA256_DIGEST_LENGTH, NULL);
}
static void UpdateHashNumber(Sha256& hasher, const BIGNUM* n)
{
    unsigned long len = BN_num_bytes(n);
    u8* n_bytes = (u8*)malloc(len);
    if (!n_bytes)
        return;
    BN_bn2bin(n, n_bytes);
    hasher.Update(n_bytes, len);
    free(n_bytes);
}
static BIGNUM* CalculateX(const BIGNUM* salt, const std::string& username, const u8* password, const size_t password_len)
{
    u8 ucp_hash[SHA256_DIGEST_LENGTH];

    Sha256 hasher;
    hasher.Init();
    {
        hasher.Update(username.c_str(), username.length());
        hasher.Update(":", 1);
        hasher.Update(password, password_len);
    }
    hasher.Final(ucp_hash);

    return HashNumberString(salt, ucp_hash, SHA256_DIGEST_LENGTH);
}
static void CalculateM(NGConstant* ng, u8* dest, const std::string I, const BIGNUM* s, const BIGNUM* A, const BIGNUM* B, const u8* K)
{
    u8 H_N[SHA256_DIGEST_LENGTH];
    u8 H_g[SHA256_DIGEST_LENGTH];
    u8 H_I[SHA256_DIGEST_LENGTH];
    u8 H_xor[SHA256_DIGEST_LENGTH];
    i32 i = 0;
    i32 hash_len = SHA256_DIGEST_LENGTH;

    HashNumber(ng->n, H_N);
    HashNumber(ng->g, H_g);

    Sha256 hasher;
    hasher.Hash((const u8*)I.c_str(), I.length(), H_I);

    for (i = 0; i < hash_len; i++)
        H_xor[i] = H_N[i] ^ H_g[i];

    hasher.Init();
    {
        hasher.Update(H_xor, hash_len);
        hasher.Update(H_I, hash_len);
        UpdateHashNumber(hasher, s);
        UpdateHashNumber(hasher, A);
        UpdateHashNumber(hasher, B);
        hasher.Update(K, hash_len);
    }
    hasher.Final(dest);
}
static void CalculateHAMK(u8* dest, const BIGNUM* A, const u8* M, const u8* K)
{
    Sha256 hasher;
    hasher.Init();
    {
        UpdateHashNumber(hasher, A);
        hasher.Update(M, SHA256_DIGEST_LENGTH);
        hasher.Update(K, SHA256_DIGEST_LENGTH);
    }
    hasher.Final(dest);
}

// Novuscore Wrappers
NGConstant::NGConstant() : n(BN_new()), g(BN_new())
{
}
NGConstant::~NGConstant()
{
    BN_free(n);
    BN_free(g);
}

void SRPUtils::CreateAccount(const std::string& username, const std::string& password, ByteBuffer* sBuffer, ByteBuffer* vBuffer)
{
    NGConstant* ng = GetNG();
    BN_CTX* ctx = BN_CTX_new();
    BIGNUM* s = BN_new();
    BIGNUM* v = BN_new();
    BIGNUM* x = nullptr;

    if (!s || !v || !ctx || !ng)
    {
        throw std::exception("Failed to CreateAccount (!s || !v || !ctx || !ng)");
    }

    RandomInit();

    BN_rand(s, 32, -1, 0);
    x = CalculateX(s, username, (const u8*)password.c_str(), password.length());

    if (!x)
    {
        throw std::exception("Failed to CreateAccount (!x)");
    }

    BN_mod_exp(v, ng->g, x, ng->n, ctx);


    size_t newSizeS = static_cast<size_t>(BN_num_bytes(s));
    size_t newSizeV = static_cast<size_t>(BN_num_bytes(v));

    assert(sBuffer->Size >= newSizeS);
    assert(vBuffer->Size >= newSizeV);

    sBuffer->Size = newSizeS;
    sBuffer->WrittenData = newSizeS;

    vBuffer->Size = newSizeV;
    vBuffer->WrittenData = newSizeV;

    BN_bn2bin(s, sBuffer->GetDataPointer());
    BN_bn2bin(v, vBuffer->GetDataPointer());

    BN_free(s);
    BN_free(v);
    BN_free(x);
    BN_CTX_free(ctx);
}

SRPUser::SRPUser() : a(BN_new()), A(BN_new()), S(BN_new()), username(""), password(""), authenticated(0)
{
    NGConstant* ng = SRPUtils::GetNG();
    SRPUtils::RandomInit();

    if (!ng || !a || !A || !S)
        throw std::exception("Failed to Initialize SRPUser (if (!ng || !a || !A || !S)");

    aBuffer = ByteBuffer::Borrow<256>();
}
SRPUser::SRPUser(const std::string& inUsername, const std::string& inPassword) : a(BN_new()), A(BN_new()), S(BN_new()), username(inUsername), password(inPassword), authenticated(0)
{
    NGConstant* ng = SRPUtils::GetNG();
    SRPUtils::RandomInit();

    if (!ng || !a || !A || !S)
        throw std::exception("Failed to Initialize SRPUser (if (!ng || !a || !A || !S)");

    aBuffer = ByteBuffer::Borrow<256>();
}
SRPUser::~SRPUser()
{
    BN_free(a);
    BN_free(A);
    BN_free(S);
}
bool SRPUser::StartAuthentication()
{
    NGConstant* ng = SRPUtils::GetNG();
    BN_CTX* ctx = BN_CTX_new();
    BN_rand(a, 256, -1, 0);
    BN_mod_exp(A, ng->g, a, ng->n, ctx);
    BN_CTX_free(ctx);

    size_t newSize = BN_num_bytes(A);
    assert(aBuffer->Size >= newSize);
    aBuffer->Size = newSize;

    // SRP-6a safety check
    if (!aBuffer->Size || aBuffer->Size < newSize)
    {
        assert(aBuffer->Size && aBuffer->Size >= newSize);
        return false;
    }

    aBuffer->WrittenData = aBuffer->Size;
    BN_bn2bin(A, aBuffer->GetDataPointer());
    return true;
}
bool SRPUser::ProcessChallenge(const u8* saltBuffer, const u8* bBuffer)
{
    // TODO: We should probably not hardcode the sizes
    NGConstant* ng = SRPUtils::GetNG();
    BIGNUM* s = BN_bin2bn(saltBuffer, 4, nullptr);
    BIGNUM* B = BN_bin2bn(bBuffer, 256, nullptr);
    BIGNUM* u = nullptr;
    BIGNUM* x = nullptr;
    BIGNUM* k = nullptr;
    BIGNUM* v = BN_new();
    BIGNUM* tmp1 = BN_new();
    BIGNUM* tmp2 = BN_new();
    BIGNUM* tmp3 = BN_new();
    BN_CTX* ctx = BN_CTX_new();

    if (!s || !B || !v || !tmp1 || !tmp2 || !tmp3 || !ctx)
    {
        assert(s && B && v && tmp1 && tmp2 && tmp3 && ctx);
        return false;
    }

    u = HashNumbers(ng->n, A, B);
    if (!u)
    {
        assert(u);
        return false;
    }

    x = CalculateX(s, username, (const u8*)password.c_str(), password.length());
    if (!x)
    {
        assert(x);
        return false;
    }

    k = HashNumbers(ng->n, ng->n, ng->g);
    if (!k)
    {
        assert(k);
        return false;
    }

    // SRP-6a safety check
    if (BN_is_zero(B) || BN_is_zero(u))
    {
        assert(!BN_is_zero(B) && !BN_is_zero(u));
        return false;
    }

    BN_mod_exp(v, ng->g, x, ng->n, ctx);

    /* S = (B - k*(g^x)) ^ (a + ux) */
    BN_mul(tmp1, u, x, ctx);
    BN_add(tmp2, a, tmp1);             /* tmp2 = (a + ux)      */
    BN_mod_exp(tmp1, ng->g, x, ng->n, ctx);
    BN_mul(tmp3, k, tmp1, ctx);             /* tmp3 = k*(g^x)       */
    BN_sub(tmp1, B, tmp3);                  /* tmp1 = (B - K*(g^x)) */
    BN_mod_exp(S, tmp1, tmp2, ng->n, ctx);

    HashNumber(S, session_key);
    CalculateM(ng, M, username.c_str(), s, A, B, session_key);
    CalculateHAMK(HAMK, A, M, session_key);

    BN_free(s);
    BN_free(B);
    BN_free(u);
    BN_free(x);
    BN_free(k);
    BN_free(v);
    BN_free(tmp1);
    BN_free(tmp2);
    BN_free(tmp3);
    BN_CTX_free(ctx);

    return true;
}
bool SRPUser::VerifySession(const u8* inHAMK)
{
    if (memcmp(HAMK, inHAMK, SHA256_DIGEST_LENGTH) != 0)
        return false;

    authenticated = 1;
    return true;
}

SRPVerifier::SRPVerifier() : authenticated(0), username()
{
    bBuffer = ByteBuffer::Borrow<256>();
    saltBuffer = nullptr;
    verifierBuffer = nullptr;
}
bool SRPVerifier::StartVerification(const std::string& inUsername, const u8* aBuffer)
{
    NGConstant* ng = SRPUtils::GetNG();

    BIGNUM* s = BN_bin2bn(saltBuffer->GetDataPointer(), static_cast<i32>(saltBuffer->Size), nullptr);
    BIGNUM* v = BN_bin2bn(verifierBuffer->GetDataPointer(), static_cast<i32>(verifierBuffer->Size), nullptr);
    BIGNUM* A = BN_bin2bn(aBuffer, 256, nullptr);
    BIGNUM* k = nullptr;
    BIGNUM* u = nullptr;
    BIGNUM* B = BN_new();
    BIGNUM* S = BN_new();
    BIGNUM* b = BN_new();
    BIGNUM* tmp1 = BN_new();
    BIGNUM* tmp2 = BN_new();
    BN_CTX* ctx = BN_CTX_new();

    username = inUsername;

    if (!s || !v || !A || !B || !S || !b || !tmp1 || !tmp2 || !ctx || !ng)
    {
        assert(s && v && A && B && S && b && tmp1 && tmp2 && ctx && ng);
        return false;
    }

    SRPUtils::RandomInit();

    // SRP-6a safety check
    BN_mod(tmp1, A, ng->n, ctx);
    if (BN_is_zero(tmp1))
    {
        assert(!BN_is_zero(tmp1));
        return false;
    }

    BN_rand(b, 256, -1, 0);

    k = HashNumbers(ng->n, ng->n, ng->g);
    if (!k)
    {
        assert(k);
        return false;
    }

    /* B = kv + g^b */
    BN_mod_mul(tmp1, k, v, ng->n, ctx);
    BN_mod_exp(tmp2, ng->g, b, ng->n, ctx);
    BN_mod_add(B, tmp1, tmp2, ng->n, ctx);

    u = HashNumbers(ng->n, A, B);
    if (!u)
    {
        assert(u);
        return false;
    }

    /* S = (A *(v^u)) ^ b */
    BN_mod_exp(tmp1, v, u, ng->n, ctx);
    BN_mul(tmp2, A, tmp1, ctx);
    BN_mod_exp(S, tmp2, b, ng->n, ctx);

    HashNumber(S, session_key);

    CalculateM(ng, M, username, s, A, B, session_key);
    CalculateHAMK(HAMK, A, M, session_key);

    size_t newSize = BN_num_bytes(B);
    bBuffer->Size = newSize;
    bBuffer->WrittenData = bBuffer->Size;

    // SRP-6a safety check
    if (!bBuffer->Size || bBuffer->Size < newSize)
    {
        assert(bBuffer->Size && bBuffer->Size >= newSize);
        return false;
    }

    BN_bn2bin(B, bBuffer->GetDataPointer());

    BN_free(s);
    BN_free(v);
    BN_free(A);
    BN_free(k);
    BN_free(u);
    BN_free(B);
    BN_free(S);
    BN_free(b);
    BN_free(tmp1);
    BN_free(tmp2);
    BN_CTX_free(ctx);
    return true;
}
bool SRPVerifier::VerifySession(const u8* inM)
{
    if (memcmp(M, inM, SHA256_DIGEST_LENGTH) != 0)
        return false;

    authenticated = 1;
    return true;
}