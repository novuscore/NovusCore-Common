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
#pragma once
#include <NovusTypes.h>
#include "Utils/StringUtils.h"
#include "Utils/JsonConfig.h"

#include "CVar/CVarSystem.h"

#include <unordered_map>
#include <array>
#include <shared_mutex>

enum class CVarType : u8
{
    INT,
    FLOAT,
    STRING,
    FLOATVEC,
    INTVEC
};

class CVarParameter
{
public:
    friend class CVarSystemImpl;

    i32 arrayIndex;

    CVarType type;
    CVarFlags flags;
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

template<typename T>
struct CVarArray
{
    std::vector<CVarStorage<T>> cvars;
    i32 lastCVar{ 0 };

    CVarArray(size_t size)
    {
        cvars.resize(size);
    }

    CVarStorage<T>* GetCurrentStorage(i32 index)
    {
        return &cvars[index];
    }

    T* GetCurrentPtr(i32 index)
    {
        return &((cvars.data() + index)->current);
    };

    T GetCurrent(i32 index)
    {
        return cvars[index].current;
    };

    void SetCurrent(const T& val, i32 index)
    {
        cvars[index].current = val;

        bool noEdit = ((u32)cvars[index].parameter->flags & (u32)CVarFlags::Noedit) != 0;
        bool readOnly = ((u32)cvars[index].parameter->flags & (u32)CVarFlags::EditReadOnly) != 0;
        if (!noEdit && !readOnly)
        {
            CVarSystemImpl::Get()->MarkDirty();
        }
    }

    int Add(const T& value, CVarParameter* param)
    {
        int index = lastCVar;

        cvars[index].current = value;
        cvars[index].initial = value;
        cvars[index].parameter = param;

        param->arrayIndex = index;
        lastCVar++;

        CVarSystemImpl::Get()->MarkDirty();
        return index;
    }

    int Add(const T& initialValue, const T& currentValue, CVarParameter* param)
    {
        int index = lastCVar;

        cvars[index].current = currentValue;
        cvars[index].initial = initialValue;
        cvars[index].parameter = param;

        param->arrayIndex = index;
        lastCVar++;

        CVarSystemImpl::Get()->MarkDirty();
        return index;
    }
};

class CVarSystemImpl : public CVarSystem
{
public:
    CVarParameter* GetCVar(StringUtils::StringHash hash) override final;

    CVarParameter* CreateFloatCVar(const char* name, const char* description, f64 defaultValue, f64 currentValue) override final;
    CVarParameter* CreateIntCVar(const char* name, const char* description, i32 defaultValue, i32 currentValue) override final;
    CVarParameter* CreateStringCVar(const char* name, const char* description, const char* defaultValue, const char* currentValue) override final;
    CVarParameter* CreateVecFloatCVar(const char* name, const char* description, const vec4& defaultValue, const vec4& currentValue)override final;
    CVarParameter* CreateVecIntCVar(const char* name, const char* description, const ivec4& defaultValue, const ivec4& currentValue)override final;

    f64* GetFloatCVar(StringUtils::StringHash hash) override final;
    i32* GetIntCVar(StringUtils::StringHash hash) override final;
    const char* GetStringCVar(StringUtils::StringHash hash) override final;
    vec4* GetVecFloatCVar(StringUtils::StringHash hash) override final;
    ivec4* GetVecIntCVar(StringUtils::StringHash hash) override final;

    void SetFloatCVar(StringUtils::StringHash hash, f64 value) override final;
    void SetIntCVar(StringUtils::StringHash hash, i32 value) override final;
    void SetStringCVar(StringUtils::StringHash hash, const char* value) override final;
    void SetVecFloatCVar(StringUtils::StringHash hash, const vec4& value) override final;
    void SetVecIntCVar(StringUtils::StringHash hash, const ivec4& value) override final;

    void DrawImguiEditor() override final;
    void LoadCVarsIntoJson(json& jsonConfig);
    void LoadCVarsFromJson(json& jsonConfig);

    void EditParameter(CVarParameter* p, float textWidth);

    constexpr static int MAX_INT_CVARS = 1000;

    CVarArray<i32> intCVars2{ MAX_INT_CVARS };

    constexpr static int MAX_FLOAT_CVARS = 1000;
    CVarArray<f64> floatCVars{ MAX_FLOAT_CVARS };

    constexpr static int MAX_STRING_CVARS = 200;
    CVarArray<std::string> stringCVars{ MAX_STRING_CVARS };

    constexpr static int MAX_FLOATVEC_CVARS = 400;
    CVarArray<vec4> fvecCVars{ MAX_FLOATVEC_CVARS };

    constexpr static int MAX_INTVEC_CVARS = 400;
    CVarArray<ivec4> ivecCVars{ MAX_INTVEC_CVARS };

    //using templates with specializations to get the cvar arrays for each type.
    //if you try to use a type that doesnt have specialization, it will trigger the static assert
    template<typename T>
    CVarArray<T>* GetCVarArray()
    {
        static_assert(false);
        return nullptr;
    }

    template<>
    CVarArray<i32>* GetCVarArray()
    {
        return &intCVars2;
    }
    template<>
    CVarArray<f64>* GetCVarArray()
    {
        return &floatCVars;
    }
    template<>
    CVarArray<std::string>* GetCVarArray()
    {
        return &stringCVars;
    }
    template<>
    CVarArray<vec4>* GetCVarArray()
    {
        return &fvecCVars;
    }
    template<>
    CVarArray<ivec4>* GetCVarArray()
    {
        return &ivecCVars;
    }

    //templated get-set cvar versions for syntax sugar
    template<typename T>
    T* GetCVarCurrent(u32 namehash) {
        auto par = GetCVar(namehash);
        return GetCVarArray<T>()->GetCurrentPtr(par->arrayIndex);
    }

    template<typename T>
    void SetCVarCurrent(u32 namehash, const T& value)
    {
        GetCVarArray<T>()->SetCurrent(value, GetCVar(namehash)->arrayIndex);
    }

    static CVarSystemImpl* Get()
    {
        return static_cast<CVarSystemImpl*>(CVarSystem::Get());
    }

private:
    std::shared_mutex mutex_;

    CVarParameter* InitCVar(const char* name, const char* description);

    std::unordered_map<u32, CVarParameter> savedCVars;

    std::vector<CVarParameter*> cachedEditParameters;
};