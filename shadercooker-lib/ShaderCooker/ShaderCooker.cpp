#include "ShaderCooker.h"
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <unordered_map>

#include <Utils/DebugHandler.h>
#include <Utils/StringUtils.h>

namespace fs = std::filesystem;

namespace ShaderCooker
{
    class IncludeHandler : public IDxcIncludeHandler
    {
    public:
        IncludeHandler(IDxcUtils* utils)
            : _utils(utils)
            , m_cRef(1)
        {
        }

        void AddIncludeDirectory(const fs::path& path)
        {
            _includeDirectories.push_back(path);
        }

        ULONG AddRef()
        {
            InterlockedIncrement(&m_cRef);
            return m_cRef;
        }

        ULONG Release()
        {
            // Decrement the object's internal counter.
            ULONG ulRefCount = InterlockedDecrement(&m_cRef);
            if (0 == m_cRef)
            {
                delete this;
            }
            return ulRefCount;
        }

    private:
        HRESULT STDMETHODCALLTYPE LoadSource(
            _In_z_ LPCWSTR pFilename,                                 // Candidate filename.
            _COM_Outptr_result_maybenull_ IDxcBlob** ppIncludeSource  // Resultant source object for included file, nullptr if not found.
        ) override
        {
            const fs::path filePath = pFilename;
            for (const fs::path& includeDir : _includeDirectories)
            {
                const fs::path fullPath = includeDir / filePath;
                if (fs::exists(fullPath))
                {
                    _utils->LoadFile(fullPath.wstring().c_str(), nullptr, (IDxcBlobEncoding**)ppIncludeSource);
                    return S_OK;
                }
            }

            *ppIncludeSource = nullptr;
            return E_FAIL;
        }

        HRESULT QueryInterface(REFIID riid, LPVOID* ppvObj)
        {
            // Always set out parameter to NULL, validating it first.
            if (!ppvObj)
                return E_INVALIDARG;
            *ppvObj = NULL;
            if (riid == IID_IUnknown || riid == __uuidof(IDxcIncludeHandler))
            {
                // Increment the reference count and return the pointer.
                *ppvObj = (LPVOID)this;
                AddRef();
                return NOERROR;
            }
            return E_NOINTERFACE;
        }

        volatile ULONG m_cRef;

        IDxcUtils* _utils;
        std::vector<fs::path> _includeDirectories;
    };

    ShaderCooker::ShaderCooker()
    {
        HRESULT r;

        r = DxcCreateInstance(CLSID_DxcUtils, IID_PPV_ARGS(_utils.GetAddressOf()));
        if (r != S_OK)
        {
            NC_LOG_FATAL("Failed to create DXC Utils");
        }

        r = DxcCreateInstance(CLSID_DxcCompiler, IID_PPV_ARGS(_compiler.GetAddressOf()));
        if (r != S_OK)
        {
            NC_LOG_FATAL("Failed to create DXC Compiler");
        }

        _includeHandler = new IncludeHandler(_utils.Get());
    }

    ShaderCooker::~ShaderCooker()
    {
        _includeHandler->Release();
    }

    void ShaderCooker::AddIncludeDir(std::filesystem::path path)
    {
        _includeHandler->AddIncludeDirectory(path);
    }

    void ShaderCooker::CompileFile(std::filesystem::path path, char*& blob, size_t& blobSize)
    {
        path.make_preferred();

        if (!fs::exists(path))
        {
            NC_LOG_FATAL("The path provided does not exists: %s", path.c_str());
        }

        if (fs::is_directory(path))
        {
            NC_LOG_FATAL("The path provided is a directory, not a file: %s", path.c_str());
        }

        std::ifstream file(path);
        std::string source((std::istreambuf_iterator<char>(file)),
            (std::istreambuf_iterator<char>()));
        file.close();

        HRESULT r;
        Microsoft::WRL::ComPtr<IDxcBlobEncoding> sourceBlob;
        r = _utils->CreateBlob(source.c_str(), static_cast<u32>(source.size()), CP_UTF8, sourceBlob.GetAddressOf());

        if (r != S_OK)
        {
            NC_LOG_FATAL("Could not load shader blob");
        }

        const wchar_t* args[] =
        {
            L"-spirv",          // Generate SPIR-V for Vulkan
            L"-fvk-use-gl-layout", // Set memory layout
            L"-Zpr",            //Row-major matrices
            L"-WX",             //Warnings as errors
            L"-enable-16bit-types", // Enable 16 bit types
    #ifdef _DEBUG
            L"-Zi",             //Debug info
            //L"-Qembed_debug", //Embed debug info into the shader
            L"-Od",             //Disable optimization
    #else
            L"-O3",             //Optimization level 3
    #endif
        };

        std::vector<DxcDefine> defines = GetDefaultDefines();

        // TODO: Permutations

        std::wstring profile;
        if (!GetProfileFromFilename(path.filename(), profile))
        {
            return;
        }

        Microsoft::WRL::ComPtr<IDxcOperationResult> compileResult;
        r = _compiler->Compile(sourceBlob.Get(), path.filename().c_str(), L"main", profile.c_str(), &args[0], sizeof(args) / sizeof(args[0]), defines.data(), static_cast<u32>(defines.size()), _includeHandler, compileResult.GetAddressOf());
        if (r != S_OK)
        {
            NC_LOG_FATAL("Compiler would not even give us back a result");
        }

        if (compileResult->GetStatus(&r) != S_OK)
        {
            NC_LOG_FATAL("Compiler gave us something but we could not get a result from it");
        }

        if (r < 0)
        {
            Microsoft::WRL::ComPtr<IDxcBlobEncoding> printBlob;
            if (compileResult->GetErrorBuffer(printBlob.GetAddressOf()) != S_OK)
            {
                NC_LOG_FATAL("Compiler gave us an error, but we could not get the text from it");
            }

            NC_LOG_ERROR("%s\n", (const char*)printBlob->GetBufferPointer());
        }

        IDxcBlob* resultBlob;
        compileResult->GetResult(&resultBlob);

        blob = (char*)resultBlob->GetBufferPointer();
        blobSize = resultBlob->GetBufferSize();

        // TODO: Success message with time taken
    }

    DxcDefine MakeDefine(const std::wstring& name, const std::wstring& value)
    {
        DxcDefine define;
        define.Name = new wchar_t[name.size()];
        wmemcpy((wchar_t*)define.Name, name.c_str(), name.size() + 1);

        define.Value = new wchar_t[value.size()];
        wmemcpy((wchar_t*)define.Value, value.c_str(), value.size() + 1);

        return define;
    }

    std::vector<DxcDefine> ShaderCooker::GetDefaultDefines()
    {
        std::vector<DxcDefine> defines;
        defines.reserve(32);

        defines.push_back(MakeDefine(L"GLOBAL", L"0"));
        defines.push_back(MakeDefine(L"PER_PASS", L"1"));
        defines.push_back(MakeDefine(L"PER_DRAW", L"2"));

        return defines;
    }

    constexpr char* validProfilesArray[9] =
    {
        "ps", // Pixel Shader
        "vs", // Vertex Shader
        "gs", // Geometry Shader
        "hs", // Hull shader
        "ds", // Domain shader
        "cs", // Compute Shader
        "lib", // Libraries, used for raytracing
        "ms", // Mesh Shader
        "as" // Amplification Shader (used with Mesh Shaders)
    };
    constexpr char* defaultProfileVersion = "_6_5";

    bool ShaderCooker::GetProfileFromFilename(std::filesystem::path filename, std::wstring& profile)
    {
        static std::string validProfiles = "";

        if (validProfiles == "")
        {
            bool first = true;
            for (auto& validProfile : validProfilesArray)
            {
                if (!first)
                {
                    validProfiles += ", ";
                }

                validProfiles += validProfile;
                first = false;
            }
        }

        // We expect filename to end with .XX.hlsl where XX is the profile of the shader, for example vs for vertex shader, ps for pixel shader, cs for compute etc
        // First we remove the .hlsl part of the name
        fs::path withoutHlsl = filename.replace_extension();

        if (!withoutHlsl.has_extension())
        {
            NC_LOG_ERROR("Filename \"%s\" should end with .XX.hlsl where XX is one of these valid profiles depending on shader type: %s", filename.string(), validProfiles);

            return false;
        }

        std::string extension = withoutHlsl.extension().string().substr(1); // Get the extension (.vs) as a string, then remove the first char which will be the "."

        if (extension.length() != 2 && extension.length() != 3)
        {
            NC_LOG_ERROR("Filename \"%s\" should end with .XX.hlsl where XX is one of these valid profiles depending on shader type: %s", filename.string(), validProfiles);

            return false;
        }

        // Make sure the profile is valid
        bool isValidProfile = false;
        for (auto& validProfile : validProfilesArray)
        {
            if (extension == validProfile)
            {
                isValidProfile = true;
                break;
            }
        }

        if (!isValidProfile)
        {
            NC_LOG_ERROR("Filename \"%s\" should end with .XX.hlsl where XX is one of these valid profiles depending on shader type: %s", filename.string(), validProfiles);
            return false;
        }

        extension = extension.append(defaultProfileVersion); // Turns the string from "vs" to "vs_6_5" or whatever version we set in defaultProfileVersion
        profile = StringUtils::StringToWString(extension);
        return true;
    }
}