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
#include <algorithm>
#include "Utils/DebugHandler.h"
#include "imgui/imgui.h"
#include "imgui/misc/cpp/imgui_stdlib.h"

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

    void DrawImguiEditor() override final;

    void EditParameter(CVarParameter* p);

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

    std::vector<CVarParameter*> cachedEditParameters;
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

void CVarSystemImpl::DrawImguiEditor()
{
    static std::string searchText = "";

    ImGui::InputText("Filter",&searchText);
    static bool bShowAdvanced = false;
    ImGui::Checkbox("Advanced", &bShowAdvanced);

    cachedEditParameters.clear();

    auto addToEditList = [&](auto parameter)
    {
        bool bHidden = ((u32)parameter->flags & (u32)CVarFlags::Noedit);
        bool bIsAdvanced = ((u32)parameter->flags & (u32)CVarFlags::Advanced);

        if (!bHidden)
        {
            if (!(!bShowAdvanced && bIsAdvanced) &&  parameter->name.find(searchText) != std::string::npos)
            {
                cachedEditParameters.push_back(parameter);
            };
        }
    };

    for (int i = 0; i < lastIntCVar; i++)
    {
        addToEditList(intCVars[i].parameter);
    }
    for (int i = 0; i < lastFloatCVar; i++)
    {
        addToEditList(floatCVars[i].parameter);
    }
    for (int i = 0; i < lastStringCVar; i++)
    {
        addToEditList(stringCVars[i].parameter);
    }
    
    if (cachedEditParameters.size() > 10)
    {
        std::unordered_map<std::string, std::vector<CVarParameter*>> categorizedParams;

        //insert all the edit parameters into the hashmap by category
        for (auto p : cachedEditParameters)
        {
            int dotPos = -1;
            //find where the first dot is to categorize
            for (int i = 0; i < p->name.length();i++)
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

        for (auto p : cachedEditParameters) {
            EditParameter(p);
        }
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
            ImGui::Text(displayFormat.c_str(), intCVars[p->arrayIndex].current);
        }
        else
        {
            if (checkboxFlag)
            {
                bool bCheckbox = intCVars[p->arrayIndex].current != 0;
                ImGui::Checkbox(p->name.c_str(), &bCheckbox);

                intCVars[p->arrayIndex].current = bCheckbox ? 1 : 0;
            }
            else
            {
                ImGui::InputInt(p->name.c_str(), &intCVars[p->arrayIndex].current);
            }
        }
    break;

    case CVarType::FLOAT:
        
        if (readonlyFlag)
        {
            std::string displayFormat = p->name + "= %f";
            ImGui::Text(displayFormat.c_str(), floatCVars[p->arrayIndex].current);
        }
        else
        {
            if (dragFlag)
            {
                ImGui::DragFloat(p->name.c_str(), &floatCVars[p->arrayIndex].current);

            }
            else
            {
                ImGui::InputFloat(p->name.c_str(), &floatCVars[p->arrayIndex].current);
            }
        }
    break;

    case CVarType::STRING:

        if (readonlyFlag)
        {
            std::string displayFormat = p->name + "= %s";
            ImGui::Text(displayFormat.c_str(), stringCVars[p->arrayIndex].current);
        }
        else
        {
            ImGui::InputText(p->name.c_str(), &stringCVars[p->arrayIndex].current);
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

CVarParameter* CVarSystemImpl::CreateFloatCVar(const char* name, const char* description, float defaultValue)
{
    CVarParameter* param = InitCVar(name, description);
    if (!param) return nullptr;

    param->type = CVarType::FLOAT;

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

AutoCVar_Float::AutoCVar_Float(const char* name, const char* description, float defaultValue, CVarFlags flags )
{
    CVarParameter* cvar = CVarSystem::Get()->CreateFloatCVar(name, description, defaultValue);
    cvar->flags = flags;
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

AutoCVar_Int::AutoCVar_Int(const char* name, const char* description, int defaultValue, CVarFlags flags)
{
    CVarParameter* cvar = CVarSystem::Get()->CreateIntCVar(name, description, defaultValue);
    cvar->flags = flags;
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

AutoCVar_String::AutoCVar_String(const char* name, const char* description, const char* defaultValue, CVarFlags flags)
{
    CVarParameter* cvar = CVarSystem::Get()->CreateStringCVar(name, description, defaultValue);
    cvar->flags = flags;
    index = cvar->arrayIndex;
}
