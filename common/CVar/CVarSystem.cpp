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
#include "Utils/JsonConfig.h"

#include <unordered_map>

#include <array>
#include <algorithm>
#include "Utils/DebugHandler.h"
#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

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

u32 Hash(const char* str)
{
    return StringUtils::fnv1a_32(str, strlen(str));
}

class CVarSystemImpl : public CVarSystem
{
public:
    CVarParameter* GetCVar(StringUtils::StringHash hash) override final;

    CVarParameter* CreateFloatCVar(const char* name, const char* description, f64 defaultValue) override final;
    CVarParameter* CreateFloatCVar(const char* name, const char* description, f64 defaultValue, f64 currentValue) override final;
    CVarParameter* CreateIntCVar(const char* name, const char* description, i32 defaultValue) override final;
    CVarParameter* CreateIntCVar(const char* name, const char* description, i32 defaultValue, i32 currentValue) override final;
    CVarParameter* CreateStringCVar(const char* name, const char* description, const char* defaultValue) override final;
    CVarParameter* CreateStringCVar(const char* name, const char* description, const char* defaultValue, const char* currentValue) override final;
    CVarParameter* CreateVecFloatCVar(const char* name, const char* description, const vec4& defaultValue)override final;
    CVarParameter* CreateVecFloatCVar(const char* name, const char* description, const vec4& defaultValue, const vec4& currentValue)override final;
    CVarParameter* CreateVecIntCVar(const char* name, const char* description, const ivec4& defaultValue)override final;
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

    void EditParameter(CVarParameter* p);

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

    CVarParameter* InitCVar(const char* name, const char* description);

    std::unordered_map<u32, CVarParameter> savedCVars;

    std::vector<CVarParameter*> cachedEditParameters;
};

f64* CVarSystemImpl::GetFloatCVar(StringUtils::StringHash hash)
{
    return GetCVarCurrent<f64>(hash);
}

i32* CVarSystemImpl::GetIntCVar(StringUtils::StringHash hash)
{
    return GetCVarCurrent<i32>(hash);
}

const char* CVarSystemImpl::GetStringCVar(StringUtils::StringHash hash)
{
    return GetCVarCurrent<std::string>(hash)->c_str();
}

vec4* CVarSystemImpl::GetVecFloatCVar(StringUtils::StringHash hash)
{
    return GetCVarCurrent<vec4>(hash);
}

ivec4* CVarSystemImpl::GetVecIntCVar(StringUtils::StringHash hash)
{
    return GetCVarCurrent<ivec4>(hash);
}

CVarSystem* CVarSystem::Get()
{
    static CVarSystemImpl cvarSys{};
    return &cvarSys;
}


CVarParameter* CVarSystemImpl::GetCVar(StringUtils::StringHash hash)
{
    auto it = savedCVars.find(hash);

    if (it != savedCVars.end())
    {
        return &(*it).second;
    }

    return nullptr;
}

void CVarSystemImpl::SetFloatCVar(StringUtils::StringHash hash, f64 value)
{
    SetCVarCurrent<f64>(hash, value);
}

void CVarSystemImpl::SetIntCVar(StringUtils::StringHash hash, i32 value)
{
    SetCVarCurrent<i32>(hash, value);
}

void CVarSystemImpl::SetStringCVar(StringUtils::StringHash hash, const char* value)
{
    SetCVarCurrent<std::string>(hash, value);
}

void CVarSystemImpl::SetVecFloatCVar(StringUtils::StringHash hash, const vec4& value)
{
    SetCVarCurrent<vec4>(hash, value);
}

void CVarSystemImpl::SetVecIntCVar(StringUtils::StringHash hash, const ivec4& value)
{
    SetCVarCurrent<ivec4>(hash, value);
}

CVarParameter* CVarSystemImpl::CreateFloatCVar(const char* name, const char* description, f64 defaultValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::FLOAT;

    GetCVarArray<f64>()->Add(defaultValue, param);

    return param;
}

CVarParameter* CVarSystemImpl::CreateFloatCVar(const char* name, const char* description, f64 defaultValue, f64 currentValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::FLOAT;

    GetCVarArray<f64>()->Add(defaultValue, currentValue, param);

    return param;
}

CVarParameter* CVarSystemImpl::CreateIntCVar(const char* name, const char* description, i32 defaultValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::INT;

    GetCVarArray<i32>()->Add(defaultValue, param);

    return param;
}

CVarParameter* CVarSystemImpl::CreateIntCVar(const char* name, const char* description, i32 defaultValue, i32 currentValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::INT;

    GetCVarArray<i32>()->Add(defaultValue, currentValue, param);

    return param;
}

CVarParameter* CVarSystemImpl::CreateStringCVar(const char* name, const char* description, const char* defaultValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::STRING;

    GetCVarArray<std::string>()->Add(defaultValue, param);

    return param;
}

CVarParameter* CVarSystemImpl::CreateStringCVar(const char* name, const char* description, const char* defaultValue, const char* currentValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::STRING;

    GetCVarArray<std::string>()->Add(defaultValue, currentValue, param);

    return param;
}

CVarParameter* CVarSystemImpl::CreateVecFloatCVar(const char* name, const char* description, const vec4& defaultValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::FLOATVEC;

    GetCVarArray<vec4>()->Add(defaultValue, param);

    return param;
}

CVarParameter* CVarSystemImpl::CreateVecFloatCVar(const char* name, const char* description, const vec4& defaultValue, const vec4& currentValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::FLOATVEC;

    GetCVarArray<vec4>()->Add(defaultValue, currentValue, param);

    return param;
}

CVarParameter* CVarSystemImpl::CreateVecIntCVar(const char* name, const char* description, const ivec4& defaultValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::INTVEC;

    GetCVarArray<ivec4>()->Add(defaultValue, param);

    return param;
}

CVarParameter* CVarSystemImpl::CreateVecIntCVar(const char* name, const char* description, const ivec4& defaultValue, const ivec4& currentValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::INTVEC;

    GetCVarArray<ivec4>()->Add(defaultValue, currentValue, param);

    return param;
}

CVarParameter* CVarSystemImpl::InitCVar(const char* name, const char* description)
{
    if (GetCVar(name)) return nullptr; //return null if the cvar already exists
    u32 namehash = StringUtils::StringHash{ name };
    savedCVars[namehash] = CVarParameter{};

    CVarParameter& newParam = savedCVars[namehash];

    newParam.name = name;
    newParam.description = description;

    return &newParam;
}

AutoCVar_Float::AutoCVar_Float(const char* name, const char* description, f64 defaultValue, CVarFlags flags)
{
    CVarParameter* cvar = CVarSystem::Get()->CreateFloatCVar(name, description, defaultValue);
    cvar->flags = flags;
    index = cvar->arrayIndex;
}

f64 AutoCVar_Float::Get()
{
    return CVarSystemImpl::Get()->GetCVarArray<f64>()->GetCurrent(index);
}

f64* AutoCVar_Float::GetPtr()
{
    return CVarSystemImpl::Get()->GetCVarArray<f64>()->GetCurrentPtr(index);
}

f32 AutoCVar_Float::GetFloat()
{
    return static_cast<f32>(Get());
}

f32* AutoCVar_Float::GetFloatPtr()
{
    f32* result = reinterpret_cast<f32*>(GetPtr());
    return result;
}

void AutoCVar_Float::Set(f64 f)
{
    CVarSystemImpl::Get()->GetCVarArray<f64>()->SetCurrent(f, index);
}

AutoCVar_Int::AutoCVar_Int(const char* name, const char* description, i32 defaultValue, CVarFlags flags)
{
    CVarParameter* cvar = CVarSystem::Get()->CreateIntCVar(name, description, defaultValue);
    cvar->flags = flags;
    index = cvar->arrayIndex;
}

i32 AutoCVar_Int::Get()
{
    return CVarSystemImpl::Get()->GetCVarArray<i32>()->GetCurrent(index);
}

i32* AutoCVar_Int::GetPtr()
{
    return CVarSystemImpl::Get()->GetCVarArray<i32>()->GetCurrentPtr(index);
}

void AutoCVar_Int::Set(i32 val)
{
    CVarSystemImpl::Get()->GetCVarArray<i32>()->SetCurrent(val, index);
}

void AutoCVar_Int::Toggle()
{
    bool enabled = Get() != 0;

    Set(enabled ? 0 : 1);
}

AutoCVar_String::AutoCVar_String(const char* name, const char* description, const char* defaultValue, CVarFlags flags)
{
    CVarParameter* cvar = CVarSystem::Get()->CreateStringCVar(name, description, defaultValue);
    cvar->flags = flags;
    index = cvar->arrayIndex;
}

const char* AutoCVar_String::Get()
{
    return CVarSystemImpl::Get()->GetCVarArray<std::string>()->GetCurrentPtr(index)->c_str();
};

void AutoCVar_String::Set(std::string&& val)
{
    CVarSystemImpl::Get()->GetCVarArray<std::string>()->SetCurrent(val, index);
}

AutoCVar_VecFloat::AutoCVar_VecFloat(const char* name, const char* description, const vec4& defaultValue, CVarFlags flags /*= CVarFlags::None*/)
{
    CVarParameter* cvar = CVarSystem::Get()->CreateVecFloatCVar(name, description, defaultValue);
    cvar->flags = flags;
    index = cvar->arrayIndex;
}

vec4 AutoCVar_VecFloat::Get()
{
    return CVarSystemImpl::Get()->GetCVarArray<vec4>()->GetCurrent(index);
}

void AutoCVar_VecFloat::Set(const vec4& val)
{
    CVarSystemImpl::Get()->GetCVarArray<vec4>()->SetCurrent(val, index);
}

AutoCVar_VecInt::AutoCVar_VecInt(const char* name, const char* description, const ivec4& defaultValue, CVarFlags flags)
{
    CVarParameter* cvar = CVarSystem::Get()->CreateVecIntCVar(name, description, defaultValue);
    cvar->flags = flags;
    index = cvar->arrayIndex;
}

ivec4 AutoCVar_VecInt::Get()
{
    return CVarSystemImpl::Get()->GetCVarArray<ivec4>()->GetCurrent(index);
}

void AutoCVar_VecInt::Set(const ivec4& val)
{
    CVarSystemImpl::Get()->GetCVarArray<ivec4>()->SetCurrent(val, index);
}

void CVarSystemImpl::DrawImguiEditor()
{
    static std::string searchText = "";

    ImGui::InputText("Filter", &searchText);
    static bool bShowAdvanced = false;
    ImGui::Checkbox("Advanced", &bShowAdvanced);

    cachedEditParameters.clear();

    auto addToEditList = [&](auto parameter)
    {
        bool bHidden = ((u32)parameter->flags & (u32)CVarFlags::Noedit);
        bool bIsAdvanced = ((u32)parameter->flags & (u32)CVarFlags::Advanced);

        if (!bHidden)
        {
            if (!(!bShowAdvanced && bIsAdvanced) && parameter->name.find(searchText) != std::string::npos)
            {
                cachedEditParameters.push_back(parameter);
            };
        }
    };

    for (int i = 0; i < GetCVarArray<i32>()->lastCVar; i++)
    {
        addToEditList(GetCVarArray<i32>()->cvars[i].parameter);
    }
    for (int i = 0; i < GetCVarArray<f64>()->lastCVar; i++)
    {
        addToEditList(GetCVarArray<f64>()->cvars[i].parameter);
    }
    for (int i = 0; i < GetCVarArray<std::string>()->lastCVar; i++)
    {
        addToEditList(GetCVarArray<std::string>()->cvars[i].parameter);
    }
    for (int i = 0; i < GetCVarArray<vec4>()->lastCVar; i++)
    {
        addToEditList(GetCVarArray<vec4>()->cvars[i].parameter);
    }
    for (int i = 0; i < GetCVarArray<ivec4>()->lastCVar; i++)
    {
        addToEditList(GetCVarArray<ivec4>()->cvars[i].parameter);
    }

    if (cachedEditParameters.size() > 10)
    {
        std::unordered_map<std::string, std::vector<CVarParameter*>> categorizedParams;

        //insert all the edit parameters into the hashmap by category
        for (auto p : cachedEditParameters)
        {
            int dotPos = -1;
            //find where the first dot is to categorize
            for (int i = 0; i < p->name.length(); i++)
            {
                if (p->name[i] == '.')
                {
                    dotPos = i;
                    break;
                }
            }
            std::string category = "";
            if (dotPos != -1)
            {
                category = p->name.substr(0, dotPos);
            }

            auto it = categorizedParams.find(category);
            if (it == categorizedParams.end())
            {
                categorizedParams[category] = std::vector<CVarParameter*>();
                it = categorizedParams.find(category);
            }
            it->second.push_back(p);
        }

        for (auto [category, parameters] : categorizedParams)
        {
            //alphabetical sort
            std::sort(parameters.begin(), parameters.end(), [](CVarParameter* A, CVarParameter* B)
                {
                    return A->name < B->name;
                });

            if (ImGui::BeginMenu(category.c_str()))
            {
                for (auto p : parameters)
                {
                    EditParameter(p);
                }

                ImGui::EndMenu();
            }
        }
    }
    else
    {
        //alphabetical sort
        std::sort(cachedEditParameters.begin(), cachedEditParameters.end(), [](CVarParameter* A, CVarParameter* B)
            {
                return A->name < B->name;
            });

        for (auto p : cachedEditParameters)
        {
            EditParameter(p);
        }
    }
}

namespace glm
{
    void to_json(json& j, const vec4& P) {
        j = { { "x", P.x }, { "y", P.y }, { "z", P.w }, { "w", P.z } };
    };

    void from_json(const json& j, vec4& P) {
        P.x = j.at("x").get<f32>();
        P.y = j.at("y").get<f32>();
        P.z = j.at("z").get<f32>();
        P.w = j.at("w").get<f32>();
    }

    void to_json(json& j, const ivec4& P) {
        j = { { "x", P.x }, { "y", P.y }, { "z", P.w }, { "w", P.z } };
    };

    void from_json(const json& j, ivec4& P) {
        P.x = j.at("x").get<i32>();
        P.y = j.at("y").get<i32>();
        P.z = j.at("z").get<i32>();
        P.w = j.at("w").get<i32>();
    }
}

void CVarSystemImpl::LoadCVarsIntoJson(json& jsonConfig)
{
    json& intConfig = jsonConfig["integer"];
    for (i32 i = 0; i < GetCVarArray<i32>()->lastCVar; i++)
    {
        CVarStorage<i32>& cvar = GetCVarArray<i32>()->cvars[i];
        CVarParameter* parameter = cvar.parameter;

        json& object = json::object();

        object["initial"] = cvar.initial;
        object["current"] = cvar.current;
        object["type"] = parameter->type;
        object["flags"] = parameter->flags;
        object["description"] = parameter->description;

        intConfig[parameter->name] = object;
    }

    json& doubleConfig = jsonConfig["double"];
    for (i32 i = 0; i < GetCVarArray<f64>()->lastCVar; i++)
    {
        CVarStorage<f64>& cvar = GetCVarArray<f64>()->cvars[i];
        CVarParameter* parameter = cvar.parameter;

        json& object = json::object();

        object["initial"] = cvar.initial;
        object["current"] = cvar.current;
        object["type"] = parameter->type;
        object["flags"] = parameter->flags;
        object["description"] = parameter->description;

        doubleConfig[parameter->name] = object;
    }

    json& stringConfig = jsonConfig["string"];
    for (i32 i = 0; i < GetCVarArray<std::string>()->lastCVar; i++)
    {
        CVarStorage<std::string>& cvar = GetCVarArray<std::string>()->cvars[i];
        CVarParameter* parameter = cvar.parameter;

        json& object = json::object();

        object["initial"] = cvar.initial;
        object["current"] = cvar.current;
        object["type"] = parameter->type;
        object["flags"] = parameter->flags;
        object["description"] = parameter->description;

        stringConfig[parameter->name] = object;
    }

    json& vector4 = jsonConfig["vector4"];
    for (i32 i = 0; i < GetCVarArray<vec4>()->lastCVar; i++)
    {
        CVarStorage<vec4>& cvar = GetCVarArray<vec4>()->cvars[i];
        CVarParameter* parameter = cvar.parameter;

        json& object = json::object();

        object["initial"] = cvar.initial;
        object["current"] = cvar.current;
        object["type"] = parameter->type;
        object["flags"] = parameter->flags;
        object["description"] = parameter->description;

        vector4[parameter->name] = object;
    }

    json& ivector4 = jsonConfig["ivector4"];
    for (i32 i = 0; i < GetCVarArray<ivec4>()->lastCVar; i++)
    {
        CVarStorage<ivec4>& cvar = GetCVarArray<ivec4>()->cvars[i];
        CVarParameter* parameter = cvar.parameter;

        json& object = json::object();

        object["initial"] = cvar.initial;
        object["current"] = cvar.current;
        object["type"] = parameter->type;
        object["flags"] = parameter->flags;
        object["description"] = parameter->description;

        ivector4[parameter->name] = object;
    }
}
void CVarSystemImpl::LoadCVarsFromJson(json& jsonConfig)
{
    u32 cvarNameHash = 0;

    json& intConfig = jsonConfig["integer"];
    for (auto it = intConfig.begin(); it != intConfig.end(); it++)
    {
        const std::string& key = it.key();
        json& value = it.value();

        cvarNameHash = StringUtils::fnv1a_32(key.c_str(), key.length());

        i32 initial = value["initial"].get<i32>();
        i32 current = value["current"].get<i32>();
        std::string& description = value["description"].get_ref<std::string&>();

        CVarParameter* parameter = GetCVar(cvarNameHash);
        if (!parameter)
        {
            parameter = CreateIntCVar(key.c_str(), description.c_str(), initial, current);
        }
        else
        {
            CVarStorage<i32>* storage = GetCVarArray<i32>()->GetCurrentStorage(parameter->arrayIndex);

            storage->initial = initial;
            storage->current = current;
            parameter->description = description;
        }

        parameter->type = value["type"].get<CVarType>();
        parameter->flags = value["flags"].get<CVarFlags>();
    }

    json& doubleConfig = jsonConfig["double"];
    for (auto it = doubleConfig.begin(); it != doubleConfig.end(); it++)
    {
        const std::string& key = it.key();
        json& value = it.value();

        cvarNameHash = StringUtils::fnv1a_32(key.c_str(), key.length());

        f64 initial = value["initial"].get<f64>();
        f64 current = value["current"].get<f64>();
        std::string& description = value["description"].get_ref<std::string&>();

        CVarParameter* parameter = GetCVar(cvarNameHash);
        if (!parameter)
        {
            parameter = CreateFloatCVar(key.c_str(), description.c_str(), initial, current);
        }
        else
        {
            CVarStorage<f64>* storage = GetCVarArray<f64>()->GetCurrentStorage(parameter->arrayIndex);

            storage->initial = initial;
            storage->current = current;
            parameter->description = description;
        }

        parameter->type = value["type"].get<CVarType>();
        parameter->flags = value["flags"].get<CVarFlags>();
    }

    json& stringConfig = jsonConfig["string"];
    for (auto it = stringConfig.begin(); it != stringConfig.end(); it++)
    {
        const std::string& key = it.key();
        json& value = it.value();

        cvarNameHash = StringUtils::fnv1a_32(key.c_str(), key.length());

        std::string& initial = value["initial"].get_ref<std::string&>();
        std::string& current = value["current"].get_ref<std::string&>();
        std::string& description = value["description"].get_ref<std::string&>();

        CVarParameter* parameter = GetCVar(cvarNameHash);
        if (!parameter)
        {
            parameter = CreateStringCVar(key.c_str(), description.c_str(), initial.c_str(), current.c_str());
        }
        else
        {
            CVarStorage<std::string>* storage = GetCVarArray<std::string>()->GetCurrentStorage(parameter->arrayIndex);

            storage->initial = initial;
            storage->current = current;
            parameter->description = description;
        }

        parameter->type = value["type"].get<CVarType>();
        parameter->flags = value["flags"].get<CVarFlags>();
    }

    json& vector4 = jsonConfig["vector4"];
    for (auto it = vector4.begin(); it != vector4.end(); it++)
    {
        const std::string& key = it.key();
        json& value = it.value();

        cvarNameHash = StringUtils::fnv1a_32(key.c_str(), key.length());

        vec4 initial = value["initial"].get<vec4>();
        vec4 current = value["current"].get<vec4>();
        std::string& description = value["description"].get_ref<std::string&>();

        CVarParameter* parameter = GetCVar(cvarNameHash);
        if (!parameter)
        {
            parameter = CreateVecFloatCVar(key.c_str(), description.c_str(), initial, current);
        }
        else
        {
            CVarStorage<vec4>* storage = GetCVarArray<vec4>()->GetCurrentStorage(parameter->arrayIndex);

            storage->initial = initial;
            storage->current = current;
            parameter->description = description;
        }

        parameter->type = value["type"].get<CVarType>();
        parameter->flags = value["flags"].get<CVarFlags>();
    }

    json& ivector4 = jsonConfig["ivector4"];
    for (auto it = ivector4.begin(); it != ivector4.end(); it++)
    {
        const std::string& key = it.key();
        json& value = it.value();

        cvarNameHash = StringUtils::fnv1a_32(key.c_str(), key.length());

        ivec4 initial = value["initial"].get<ivec4>();
        ivec4 current = value["current"].get<ivec4>();
        std::string& description = value["description"].get_ref<std::string&>();

        CVarParameter* parameter = GetCVar(cvarNameHash);
        if (!parameter)
        {
            parameter = CreateVecIntCVar(key.c_str(), description.c_str(), initial, current);
        }
        else
        {
            CVarStorage<ivec4>* storage = GetCVarArray<ivec4>()->GetCurrentStorage(parameter->arrayIndex);

            storage->initial = initial;
            storage->current = current;
            parameter->description = description;
        }

        parameter->type = value["type"].get<CVarType>();
        parameter->flags = value["flags"].get<CVarFlags>();
    }
}

void CVarSystemImpl::EditParameter(CVarParameter* p)
{
    const bool readonlyFlag = ((u32)p->flags & (u32)CVarFlags::EditReadOnly);
    const bool checkboxFlag = ((u32)p->flags & (u32)CVarFlags::EditCheckbox);
    const bool dragFlag = ((u32)p->flags & (u32)CVarFlags::EditFloatDrag);

    switch (p->type)
    {
    case CVarType::INT:

        if (readonlyFlag)
        {
            std::string displayFormat = p->name + "= %i";
            ImGui::Text(displayFormat.c_str(), GetCVarArray<i32>()->GetCurrent(p->arrayIndex));
        }
        else
        {
            if (checkboxFlag)
            {
                bool bCheckbox = GetCVarArray<i32>()->GetCurrent(p->arrayIndex) != 0;
                if (ImGui::Checkbox(p->name.c_str(), &bCheckbox))
                    GetCVarArray<i32>()->SetCurrent(bCheckbox ? 1 : 0, p->arrayIndex);

                // intCVars[p->arrayIndex].current = bCheckbox ? 1 : 0;
            }
            else
            {
                if (ImGui::InputInt(p->name.c_str(), GetCVarArray<i32>()->GetCurrentPtr(p->arrayIndex)))
                    MarkDirty();
            }
        }
        break;

    case CVarType::FLOAT:

        if (readonlyFlag)
        {
            std::string displayFormat = p->name + "= %f";
            ImGui::Text(displayFormat.c_str(), GetCVarArray<f64>()->GetCurrent(p->arrayIndex));
        }
        else
        {
            if (dragFlag)
            {
                if (ImGui::InputDouble(p->name.c_str(), GetCVarArray<f64>()->GetCurrentPtr(p->arrayIndex)))
                    MarkDirty();
            }
            else
            {
                if (ImGui::InputDouble(p->name.c_str(), GetCVarArray<f64>()->GetCurrentPtr(p->arrayIndex)))
                    MarkDirty();
            }
        }
        break;

    case CVarType::FLOATVEC:

        if (ImGui::InputFloat4(p->name.c_str(), &(*GetCVarArray<vec4>()->GetCurrentPtr(p->arrayIndex))[0]))
            MarkDirty();

        break;

    case CVarType::INTVEC:

        if (ImGui::InputInt4(p->name.c_str(), &(*GetCVarArray<ivec4>()->GetCurrentPtr(p->arrayIndex))[0]))
            MarkDirty();

        break;

    case CVarType::STRING:

        if (readonlyFlag)
        {
            std::string displayFormat = p->name + "= %s";
            ImGui::Text(displayFormat.c_str(), GetCVarArray<std::string>()->GetCurrent(p->arrayIndex));
        }
        else
        {
            if (ImGui::InputText(p->name.c_str(), GetCVarArray<std::string>()->GetCurrentPtr(p->arrayIndex)))
                MarkDirty();
        }
        break;

    default:
        break;
    }

    if (ImGui::IsItemHovered())
    {
        ImGui::SetTooltip(p->description.c_str());
    }
}