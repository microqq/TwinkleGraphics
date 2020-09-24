

#include "twShaderManager.h"
#include "twResourceManager.h"

namespace TwinkleGraphics
{
    ShaderManager& ShaderMgrInstance() { return Singleton<ShaderManager>::Instance(); }

    ShaderManager::ShaderManager()
        : IUpdatable()
        , INonCopyable()
        , _futures()
        , _programFutures()
        , _mutex()
        , _programMutex()
    {
    }

    ShaderManager::~ShaderManager()
    {
        Destroy();
    }

    void ShaderManager::Update()
    {
        // try
        {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                RemoveFutures<Shader>(_futures);
            }
            {
                std::lock_guard<std::mutex> lock(_programMutex);
                RemoveFutures<ShaderProgram>(_programFutures);
            }
        }
    }

    void ShaderManager::Destroy()
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _futures.clear();
        }

        {
            std::lock_guard<std::mutex> lock(_programMutex);
            _programFutures.clear();
        }
    }

    Shader::Ptr ShaderManager::ReadShader(const char* filename, ShaderOption* option)
    {
        ResourceManager& resMgr = ResourceMgrInstance();

        std::string shaderFilename(filename);
        std::string macros;
        for(int i = 0, count = option->_optionData.numMacros; i < count; i++)
        {
            macros += std::string(option->_optionData.macros[i]);
        }
        if(!macros.empty())
        {
            shaderFilename += std::string(":") + std::to_string(std::hash<std::string>{}(macros));
        }

        ReadResult<Shader> result = resMgr.Read<ShaderReader, Shader, ShaderOption>(shaderFilename.c_str(), option);
        Shader::Ptr sharedShader = result.GetSharedObject();

        return sharedShader;
    }

    ShaderProgram::Ptr ShaderManager::ReadShaders(ShaderOption options[], int32 num)
    {
        //Todo: read program from cache first

        //read shader source
        bool readyLink = true;
        ShaderProgram::Ptr program = std::make_shared<ShaderProgram>(num);
        for (int i = 0; i < num; i++)
        {
            Shader::Ptr shader = ReadShader(options[i]._optionData.filename.c_str(), &options[i]);
            if(shader != nullptr)
            {
                program->AddShader(shader);
            }
        }

        program->Link();

        return program;
    }

    ReadResult<Shader> ShaderManager::ReadShaderAsync(const char *filename, ShaderOption *option)
    {
        ResourceManager& resMgr = ResourceMgrInstance();
        option->AddSuccessFunc(0, this, &ShaderManager::OnReadShaderSuccess);
        option->AddFailedFunc(0, this, &ShaderManager::OnReadShaderFailed);

        std::string shaderFilename(filename);
        std::string macros;
        for(int i = 0, count = option->_optionData.numMacros; i < count; i++)
        {
            macros += std::string(option->_optionData.macros[i]);
        }
        if(!macros.empty())
        {
            shaderFilename += std::string(":") + std::to_string(std::hash<std::string>{}(macros));
        }
        return resMgr.ReadAsync<ShaderReader, Shader, ShaderOption>(shaderFilename.c_str(), option);
    }

    ReadResult<ShaderProgram> ShaderManager::ReadShadersAsync(ShaderProgramOption* option, int32 num)
    {
        ResourceManager& resMgr = ResourceMgrInstance();
        option->AddSuccessFunc(0, this, &ShaderManager::OnReadShadersSuccess);

        std::string programFilename;
        for(int i = 0; i < num; i++)
        {
            auto path = option->_shaderOptions[i]._optionData.filename;
            int pos = path.find_last_of("/");
            if(pos == std::string::npos)
            {
                pos = 0;
            }
            else
            {
                pos += 1;
            }
            
            auto filename = path.substr(pos);
            programFilename += ((i < num -1) ? (filename + "/") : filename);
        }

        if(!option->_macros.empty())
        {
            programFilename += std::string(":") + std::to_string(std::hash<std::string>{}(option->_macros));
        }

        return resMgr.ReadAsync<ShaderReader, ShaderProgram, ShaderProgramOption>(programFilename.c_str(), option);
    }    

    void ShaderManager::AddTaskFuture(std::future<ReadResult<Shader>> future)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _futures.emplace_back(std::move(future));
        }
    }

    void ShaderManager::AddTaskProgramFuture(std::future<ReadResult<ShaderProgram>> future)
    {
        {
            std::lock_guard<std::mutex> lock(_programMutex);
            _programFutures.emplace_back(std::move(future));
        }
    }

    void ShaderManager::OnReadShaderSuccess(Object::Ptr obj)
    {
        Shader *shader = dynamic_cast<Shader *>(obj.get());
        if (shader != nullptr)
        {
            shader->SetupCompile();
            shader->Compile();

            shader->SetValid(true);
        }
    }

    void ShaderManager::OnReadShaderFailed() 
    {}

    void ShaderManager::OnReadShadersSuccess(Object::Ptr obj)
    {
        ShaderProgram::Ptr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
        if (program != nullptr)
        {
            program->Link();
            program->SetValid(true);
        }
    }

    void ShaderManager::OnReadShadersFailed()
    {
    }

    template <>
    void ResourceManager::PackedReadTask<ReadResult<Shader>, ShaderReader>::PushTask()
    {
        // typedef Ret(R::*)(const char*, ReaderOption) Func;
        ResourceManager &resMgr = ResourceMgrInstance();
        if(_asyncRead)
        {
            auto future = resMgr.PushAsyncTask(&ShaderReader::ReadAsync, _reader, _filename);
            {
                ShaderManager& shaderMgr = ShaderMgrInstance();
                shaderMgr.AddTaskFuture(std::move(future));
            }
        }
        else
        {
            resMgr.PushTask(&ShaderReader::Read, _reader, _filename.c_str());
        }
    }

    template <>
    void ResourceManager::PackedReadTask<ReadResult<ShaderProgram>, ShaderReader>::PushTask()
    {
        // typedef Ret(R::*)(const char*, ReaderOption) Func;
        ResourceManager &resMgr = ResourceMgrInstance();
        if(_asyncRead)
        {
            auto future = resMgr.PushAsyncTask(&ShaderReader::ReadProgramAsync, _reader, _filename);
            {
                ShaderManager& shaderMgr = ShaderMgrInstance();
                shaderMgr.AddTaskProgramFuture(std::move(future));
            }
        }
    }



} // namespace TwinkleGraphics
