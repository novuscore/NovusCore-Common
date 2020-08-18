/*
# MIT License

# Copyright(c) 2018-2019 NovusCore

# Permission is hereby granted, free of charge, to any person obtaining a copy
# of this software and associated documentation files(the "Software"), to deal
# in the Software without restriction, including without limitation the rights
# to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
# copies of the Software, and to permit persons to whom the Software is
# furnished to do so, subject to the following conditions :

# The above copyright notice and this permission notice shall be included in all
# copies or substantial portions of the Software.

# THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
# IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
# FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.IN NO EVENT SHALL THE
# AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
# LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
# OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
# SOFTWARE.
*/

#include "CVarSystem.h"
#include <unordered_map>
#include "Utils/StringUtils.h"

#include <iostream>
#include "Utils/DebugHandler.h"

// FNV-1a 32bit hashing algorithm.
constexpr u32 fnv1a_32(char const* s, std::size_t count)
{
    return ((count ? fnv1a_32(s, count - 1) : 2166136261u) ^ s[count]) * 16777619u;
}

enum class CVarType : u8 {
    INT,
    FLOAT,
    STRING
};

class CVarParameter {
    friend class CVarSystemImpl;
public:
    //getters, returns null if type doesnt match
    float* AsFloat();
    int* AsInt();
    const char* AsString();

    //setters. If the type does not match, they will do nothing
    void SetFloat(float f);
    void SetInt(int i);
    void SetString(const char* str);

private:
    //just the types instead of a union for simplicity
    std::string stringParam;
    float floatParam;
    int intParam;

    CVarType type;

    std::string name;
    std::string description;
};

class CVarSystemImpl : public CVarSystem {
public:
    CVarParameter* GetCVar(const char* name) override final;
    CVarParameter* GetCVar(u32 namehash) override final;

    CVarParameter* CreateFloatCVar(const char* name, const char* description, float defaultValue) override final;
    CVarParameter* CreateIntCVar(const char* name, const char* description, int defaultValue) override final;
    CVarParameter* CreateStringCVar(const char* name, const char* description, const char* defaultValue) override final;

    float* GetFloatCVar(const char* name) override final;
    int* GetIntCVar(const char* name) override final;
    const char* GetStringCVar(const char* name) override final;

    float* GetFloatCVar(u32 namehash) override final; 
    int* GetIntCVar(u32 namehash) override final;
    const char* GetStringCVar(u32 namehash) override final;


    void SetFloatCVar(const char* name, float value) override final;
    void SetFloatCVar(u32 namehash, float value) override final;

    void SetIntCVar(const char* name, int value) override final;
    void SetIntCVar(u32 namehash, int value) override final;

    void SetStringCVar(const char* name, const char* value) override final;
    void SetStringCVar(u32 namehash, const char* value) override final;

private:

    CVarParameter* InitCVar(const char* name, const char* description);

    std::unordered_map<u32, CVarParameter> savedCVars;
};

float* CVarSystemImpl::GetFloatCVar(const char* name) 
{
    u32 namehash = fnv1a_32(name, strlen(name));
    return GetFloatCVar(namehash);
}
int* CVarSystemImpl::GetIntCVar(const char* name)
{
    u32 namehash = fnv1a_32(name, strlen(name));
    return GetIntCVar(namehash);
}
const char* CVarSystemImpl::GetStringCVar(const char* name)
{
    u32 namehash = fnv1a_32(name, strlen(name));
    return GetStringCVar(namehash);
}

float* CVarSystemImpl::GetFloatCVar(u32 namehash)
{
    auto par = GetCVar(namehash);
    return par->AsFloat();
}
int* CVarSystemImpl::GetIntCVar(u32 namehash)
{
    auto par = GetCVar(namehash);
    return par->AsInt();
}
const char* CVarSystemImpl::GetStringCVar(u32 namehash)
{
    auto par = GetCVar(namehash);
    return par->AsString();
}

CVarSystem* CVarSystem::Get()
{
    static CVarSystemImpl cvarSys{};
    return &cvarSys;
}

float* CVarParameter::AsFloat()
{
    if (type == CVarType::FLOAT)
    {
        return &floatParam;
    }
    else {
        return nullptr;
    }
}


int* CVarParameter::AsInt()
{
    if (type == CVarType::INT)
    {
        return &intParam;
    }
    else {
        return nullptr;
    }
}

const char* CVarParameter::AsString()
{
    if (type == CVarType::STRING)
    {
        return stringParam.c_str();
    }
    else {
        return nullptr;
    }
}


void CVarParameter::SetFloat(float f)
{
    if (type == CVarType::FLOAT)
    {
        floatParam = f;
    }
}

void CVarParameter::SetInt(int i)
{
    if (type == CVarType::INT)
    {
        if (intParam != i)
        {
            intParam = i;
            NC_LOG_MESSAGE("[CVAR] Changing CVAR: %s = %i",name.c_str(), i);
        }
       
    }
}


void CVarParameter::SetString(const char* str)
{
    if (type == CVarType::STRING)
    {
        stringParam = str;
    }
}

CVarParameter* CVarSystemImpl::GetCVar(const char* name)
{
    u32 namehash = fnv1a_32(name, strlen(name));
    return GetCVar(namehash);
}

CVarParameter* CVarSystemImpl::GetCVar(u32 namehash)
{
    auto it = savedCVars.find(namehash);

    if (it != savedCVars.end())
    {
        return &(*it).second;
    }

    return nullptr;
}



void CVarSystemImpl::SetFloatCVar(const char* name, float value)
{
    u32 namehash = fnv1a_32(name, strlen(name));
    SetFloatCVar(namehash, value);
}
void CVarSystemImpl::SetFloatCVar(u32 namehash, float value)
{
    GetCVar(namehash)->SetFloat(value);
}

void CVarSystemImpl::SetIntCVar(const char* name, int value)
{
    u32 namehash = fnv1a_32(name, strlen(name));
    SetIntCVar(namehash, value);
}

void CVarSystemImpl::SetIntCVar(u32 namehash, int value)
{
    GetCVar(namehash)->SetInt(value);
}

void CVarSystemImpl::SetStringCVar(const char* name, const char* value)
{
    u32 namehash = fnv1a_32(name, strlen(name));
    SetStringCVar(namehash, value);
}

void CVarSystemImpl::SetStringCVar(u32 namehash, const char* value)
{
    GetCVar(namehash)->SetString(value);
}
CVarParameter* CVarSystemImpl::CreateFloatCVar(const char* name, const char* description, float defaultValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::FLOAT;
    param->SetFloat(defaultValue);

    return param;
}

CVarParameter* CVarSystemImpl::CreateIntCVar(const char* name, const char* description, int defaultValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::INT;
    param->SetInt(defaultValue);

    return param;
}

CVarParameter* CVarSystemImpl::CreateStringCVar(const char* name, const char* description, const char* defaultValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::STRING;
    param->SetString(defaultValue);

    return param;
}

CVarParameter* CVarSystemImpl::InitCVar(const char* name, const char* description)
{
    if (GetCVar(name)) return nullptr; //return null if the cvar already exists
    u32 namehash = fnv1a_32(name, strlen(name));
    savedCVars[namehash] = CVarParameter{};

    CVarParameter& newParam = savedCVars[namehash];

    newParam.name = name;
    newParam.description = description;

    return &newParam;
}

AutoCVar_Float::AutoCVar_Float(const char* name, const char* description, float defaultValue)
{
    cvar= CVarSystem::Get()->CreateFloatCVar(name, description, defaultValue);
}


float AutoCVar_Float::Get()
{
    return *cvar->AsFloat();
}


void AutoCVar_Float::Set(float f)
{
    cvar->SetFloat(f);
}

AutoCVar_Int::AutoCVar_Int(const char* name, const char* description, int defaultValue)
{
    cvar = CVarSystem::Get()->CreateIntCVar(name, description, defaultValue);
}

int AutoCVar_Int::Get()
{
    return *cvar->AsInt();
}

void AutoCVar_Int::Set(int val)
{
    cvar->SetInt(val);
}


void AutoCVar_Int::Toggle()
{
    bool enabled = Get() != 0;

    Set(enabled ? 0 : 1);
}

AutoCVar_String::AutoCVar_String(const char* name, const char* description, const char* defaultValue)
{
    cvar = CVarSystem::Get()->CreateStringCVar(name, description, defaultValue);
}
