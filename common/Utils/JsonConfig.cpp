#include "JsonConfig.h"
#include "../Utils/DebugHandler.h"

#include <iostream>
#include <fstream>

bool JsonConfig::Load(const fs::path& configPath)
{
    std::ifstream configFile(configPath, std::ifstream::in);
    if (!configFile)
    {
        NC_LOG_ERROR("Failed to open %s. Check admin permissions", configPath.filename().string().c_str());
        return false;
    }

    configFile >> _configFile;
    configFile.close();

    if (_configFile.size() == 0)
    {
        NC_LOG_FATAL("Failed to initialize config file, found 0 configurations.");
        return false;
    }

    NC_LOG_SUCCESS("Loaded config file: '%s'", configPath.filename().string().c_str());
    return true;
}

bool JsonConfig::LoadOrCreate(const fs::path& configPath, json& configDefault)
{
    if (!fs::exists(configPath))
    {
        std::ofstream configStream(configPath, std::ofstream::out);
        if (!configStream)
        {
            NC_LOG_ERROR("Failed to create json config file. Check admin permissions");
            return false;
        }
       
        configStream << configDefault.dump(4);
        configStream.close();
    }

    return Load(configPath);
}

bool JsonConfig::Save(const fs::path& configPath)
{
    std::ofstream configFile(configPath);
    if (!configFile)
    {
        NC_LOG_ERROR("Failed to save %s. Check admin permissions", configPath.filename().string().c_str());
        return false;
    }

    configFile << _configFile.dump(4);
    configFile.close();

    return true;
}

json::value_type JsonConfig::FindOptionInArray(std::string optionName, json::value_type value)
{
    for (auto& itr : value.items())
    {
        if (itr.key() == optionName)
            return itr.value();

        if (itr.value().is_structured())
        {
            return FindOptionInArray(optionName, itr.value());
        }
    }

    return json::value_type();
}