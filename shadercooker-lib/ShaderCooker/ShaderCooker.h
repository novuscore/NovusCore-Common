#pragma once
#include <string>
#include <filesystem>

// NOTE: This will need edits to add Linux support to ShaderCooker
#ifdef _WINDOWS
#include <wrl/client.h>
#endif

#include "dxcapi.h"

namespace ShaderCooker
{
    class IncludeHandler;

    class ShaderCooker
    {
    public:
        ShaderCooker();
        ~ShaderCooker();

        void AddIncludeDir(std::filesystem::path path);
        void CompileFile(std::filesystem::path path, char*& blob, size_t& blobSize);

    private:
        std::vector<DxcDefine> GetDefaultDefines();
        bool GetProfileFromFilename(std::filesystem::path filename, std::wstring& profile);

    private:
        IncludeHandler* _includeHandler;

        Microsoft::WRL::ComPtr<IDxcUtils> _utils;
        Microsoft::WRL::ComPtr<IDxcCompiler> _compiler;
    };
}