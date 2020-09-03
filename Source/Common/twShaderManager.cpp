
#include "twShaderManager.h"
#include "twResourceManager.h"

namespace TwinkleGraphics
{
    ShaderManager& ShaderMgrInstance() { return Singleton<ShaderManager>::Instance(); }

    ShaderManager::ShaderManager()
        : IUpdatable() 
        , _futures()
        , _shaders()
        , _mutex()
    {
    }

    ShaderManager::~ShaderManager()
    {
        _futures.clear();
        _shaders.clear();
    }

    void ShaderManager::Update()
    {
        // try
        {
            std::lock_guard<std::mutex> lock(_mutex);
            for (auto &f : _futures)
            {
                if (f.valid() && TaskFinished(f))
                {
                    ReadResult<Shader> result = f.get();
                    result.OnReadSuccess();
                    result.OnReadFailed();

                    _shaders.emplace_back(result.GetSharedObject());
                }
                else //loading or wait load?
                {
                }
            }
        }
        // catch (...)
        // {
        //     std::cerr << "caught ex [1]" << '\n';
        // }
    }

    Shader::Ptr ShaderManager::ReadShader(const char* filename, ShaderOption* option)
    {
        ResourceManager& resMgr = ResourceMgrInstance();
        ReadResult<Shader> result = resMgr.Read<ShaderReader, Shader>(filename, option);
        Shader::Ptr sharedShader = result.GetSharedObject();

        return sharedShader;
    }

    ShaderProgram::Ptr ShaderManager::ReadShaders(ShaderOption options[], int32 num)
    {
        //Todo: read program from cache first

        //read shader source
        bool readyLink = true;
        Shader::Ptr* shaders = new Shader::Ptr[num];
        for (int i = 0; i < num; i++)
        {
            Shader::Ptr shader = ReadShader(options[i].optionData.filename.c_str(), &options[i]);
            if(shader != nullptr)
            {
                shaders[i] = shader;
                readyLink &= shaders[i]->Compiled();
            }
        }

        //if cache not found, create new program
        ShaderProgram::Ptr program = std::make_shared<ShaderProgram>(shaders, num);
        if (readyLink)
        {
            program->Link();
        }
        SAFE_DEL_ARR(shaders);

        return program;
    }

    void ShaderManager::ReadShaderAsync(const char *filename, ShaderOption *option)
    {
        ResourceManager& resMgr = ResourceMgrInstance();
        auto future = resMgr.ReadAsync<ShaderReader, Shader>(filename, option);

        // if(future.valid())
        // {
        //     ReadResult<Shader> result = future.get();
        //     Console::LogWarning("Wait shader load complete.\n");
        // }

        {
            std::lock_guard<std::mutex> lock(_mutex);
            _futures.push_back(std::move(future));
        }
    }

    void ShaderManager::ReadShadersAsync(ShaderOption options[], int32 num)
    {
        for(int i = 0; i < num; i++)
        {
            ReadShaderAsync(options[i].optionData.filename.c_str(), &options[i]);
        }
    }    
} // namespace TwinkleGraphics
