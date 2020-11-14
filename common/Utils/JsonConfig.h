  
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
#pragma once

#include "json.hpp"
#include <filesystem>

namespace fs = std::filesystem;

using json = nlohmann::json;
class JsonConfig
{
public:
    JsonConfig() {}

    bool Load(const fs::path& configPath);
    bool LoadOrCreate(const fs::path& configPath, json& configDefault);

    bool Save(const fs::path& configPath);

    template <class T>
    T& GetOption(std::string optionName, T defaultValue)
    {
        json::value_type value;

        for (auto& itr : _configFile.items())
        {
            if (itr.key() == optionName)
            {
                value = itr.value();
                break;
            }

            if (itr.value().is_structured())
            {
                json::value_type tempValue = FindOptionInArray(optionName, itr.value());
                if (!tempValue.is_null())
                {
                    value = tempValue;
                    break;
                }
            }
        }

        if (value.is_null())
            return defaultValue;

        try
        {
            return value.get<T>();
        }
        catch (nlohmann::detail::exception e)
        {
            std::cout << "ERROR: " << e.what() << std::endl;
            return defaultValue;
        }
    }

    json& GetConfig() { return _configFile; }
    json& GetJsonObjectByKey(std::string keyName)
    {
        return _configFile[keyName];
    }

private:
    json::value_type FindOptionInArray(std::string optionName, json::value_type value);

    json _configFile;
};