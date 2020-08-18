/*
# MIT License

# Copyright(c) 2018-2020 NovusCore

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

#include <array>
#include "Utils/DebugHandler.h"

enum class CVarType : u8 
{
    INT,
    FLOAT,
    STRING
};

class CVarParameter {
public:
    friend class CVarSystemImpl;

    int arrayIndex;

    CVarType type;

    std::string name;
    std::string description;
};

template<typename T>
struct CVarStorage
{
    T initial;
    T current;
    CVarParameter* parameter;
};

class CVarSystemImpl : public CVarSystem 
{
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

    constexpr static int max_int_cvars = 1000;
    std::array<CVarStorage<int>, max_int_cvars> intCVars;
    int lastIntCVar = 0;

    constexpr static int max_float_cvars = 1000;
    std::array<CVarStorage<float>, max_float_cvars> floatCVars;
    int lastFloatCVar = 0;

    constexpr static int max_string_cvars = 200;
    std::array<CVarStorage<std::string>, max_string_cvars> stringCVars;
    int lastStringCVar = 0;

private:

    CVarParameter* InitCVar(const char* name, const char* description);

    std::unordered_map<u32, CVarParameter> savedCVars;
};

float* CVarSystemImpl::GetFloatCVar(const char* name) 
{
    u32 namehash = StringUtils::fnv1a_32(name, strlen(name));
    return GetFloatCVar(namehash);
}
int* CVarSystemImpl::GetIntCVar(const char* name)
{
    u32 namehash = StringUtils::fnv1a_32(name, strlen(name));
    return GetIntCVar(namehash);
}
const char* CVarSystemImpl::GetStringCVar(const char* name)
{
    u32 namehash = StringUtils::fnv1a_32(name, strlen(name));
    return GetStringCVar(namehash);
}

float* CVarSystemImpl::GetFloatCVar(u32 namehash)
{
    auto par = GetCVar(namehash);
    return &floatCVars[par->arrayIndex].current;
}
int* CVarSystemImpl::GetIntCVar(u32 namehash)
{
    auto par = GetCVar(namehash);
    return &intCVars[par->arrayIndex].current;
}
const char* CVarSystemImpl::GetStringCVar(u32 namehash)
{
    auto par = GetCVar(namehash);
    return stringCVars[par->arrayIndex].current.c_str();
}

CVarSystem* CVarSystem::Get()
{
    static CVarSystemImpl cvarSys{};
    return &cvarSys;
}

CVarParameter* CVarSystemImpl::GetCVar(const char* name)
{
    u32 namehash = StringUtils::fnv1a_32(name, strlen(name));
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
    u32 namehash = StringUtils::fnv1a_32(name, strlen(name));
    SetFloatCVar(namehash, value);
}
void CVarSystemImpl::SetFloatCVar(u32 namehash, float value)
{
    floatCVars[GetCVar(namehash)->arrayIndex].current = value;
}

void CVarSystemImpl::SetIntCVar(const char* name, int value)
{
    u32 namehash = StringUtils::fnv1a_32(name, strlen(name));
    SetIntCVar(namehash, value);
}

void CVarSystemImpl::SetIntCVar(u32 namehash, int value)
{
    intCVars[GetCVar(namehash)->arrayIndex].current = value;

}

void CVarSystemImpl::SetStringCVar(const char* name, const char* value)
{
    u32 namehash = StringUtils::fnv1a_32(name, strlen(name));
    SetStringCVar(namehash, value);
}

void CVarSystemImpl::SetStringCVar(u32 namehash, const char* value)
{
    stringCVars[GetCVar(namehash)->arrayIndex].current = value;
}
CVarParameter* CVarSystemImpl::CreateFloatCVar(const char* name, const char* description, float defaultValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::FLOAT;
    //param->SetFloat(defaultValue);

    floatCVars[lastFloatCVar].parameter = param;
    floatCVars[lastFloatCVar].initial = defaultValue;
    floatCVars[lastFloatCVar].current = defaultValue;

    param->arrayIndex = lastFloatCVar;
    lastFloatCVar++;

    return param;
}

CVarParameter* CVarSystemImpl::CreateIntCVar(const char* name, const char* description, int defaultValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::INT;

    intCVars[lastIntCVar].parameter = param;
    intCVars[lastIntCVar].initial = defaultValue;
    intCVars[lastIntCVar].current = defaultValue;

    param->arrayIndex = lastIntCVar;
    lastIntCVar++;

    return param;
}

CVarParameter* CVarSystemImpl::CreateStringCVar(const char* name, const char* description, const char* defaultValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::STRING;

    stringCVars[lastStringCVar].parameter = param;
    stringCVars[lastStringCVar].initial = defaultValue;
    stringCVars[lastStringCVar].current = defaultValue;

    param->arrayIndex = lastStringCVar;
    lastStringCVar++;

    return param;
}

CVarParameter* CVarSystemImpl::InitCVar(const char* name, const char* description)
{
    if (GetCVar(name)) return nullptr; //return null if the cvar already exists
    u32 namehash = StringUtils::fnv1a_32(name, strlen(name));
    savedCVars[namehash] = CVarParameter{};

    CVarParameter& newParam = savedCVars[namehash];

    newParam.name = name;
    newParam.description = description;

    return &newParam;
}

AutoCVar_Float::AutoCVar_Float(const char* name, const char* description, float defaultValue)
{
    CVarParameter* cvar = CVarSystem::Get()->CreateFloatCVar(name, description, defaultValue);
    index = cvar->arrayIndex;
}


float AutoCVar_Float::Get()
{
    return static_cast<CVarSystemImpl*>( CVarSystem::Get())->floatCVars[index].current;
}


void AutoCVar_Float::Set(float f)
{
    static_cast<CVarSystemImpl*>(CVarSystem::Get())->floatCVars[index].current = f;
}

AutoCVar_Int::AutoCVar_Int(const char* name, const char* description, int defaultValue)
{
    CVarParameter* cvar = CVarSystem::Get()->CreateIntCVar(name, description, defaultValue);
    index = cvar->arrayIndex;
}

int AutoCVar_Int::Get()
{
    return static_cast<CVarSystemImpl*>(CVarSystem::Get())->intCVars[index].current;
}

void AutoCVar_Int::Set(int val)
{
    static_cast<CVarSystemImpl*>(CVarSystem::Get())->intCVars[index].current = val;
}


void AutoCVar_Int::Toggle()
{
    bool enabled = Get() != 0;

    Set(enabled ? 0 : 1);
}

AutoCVar_String::AutoCVar_String(const char* name, const char* description, const char* defaultValue)
{
    CVarParameter* cvar = CVarSystem::Get()->CreateStringCVar(name, description, defaultValue);
    index = cvar->arrayIndex;
}
