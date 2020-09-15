

#include "twShaderManager.h"
#include "twResourceManager.h"

namespace TwinkleGraphics
{
    ShaderManager& ShaderMgrInstance() { return Singleton<ShaderManager>::Instance(); }

    ShaderManager::ShaderManager()
        : IUpdatable()
        , INonCopyable()
        , _futures()
        , _mutex()
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

            // _futures.erase(
            //     std::remove_if(_futures.begin(), _futures.end(), [this](Future& future)
            //     {
            //         bool ret = future.valid() && TaskFinished(future);
            //         if(ret)
            //         {
            //             ReadResult<Shader> result = future.get();
            //             ResourceReader::Ptr reader = result.GetReader();
            //             if(reader != nullptr)
            //             {
            //                 // Console::LogWarning("ShaderReader use count: ", reader.use_count(), " \n");
            //                 ReaderOption* option = reader->GetReaderOption();
            //                 if(option!= nullptr)
            //                 {
            //                     Shader::Ptr shader = result.GetSharedObject();
            //                     ReadStatus status = result.GetStatus();
                            
            //                     if(ReadStatus::SUCCESS == status)
            //                     {
            //                         option->OnReadSuccess(shader);
            //                     }
            //                     else if(ReadStatus::FAILED == status)
            //                     {
            //                         option->OnReadFailed();
            //                     }
            //                 }
            //             }
            //         }
            //         else //loading or wait load?
            //         {}

            //         return ret;
            //     })
            //     , _futures.end()
            // );

            // _programFutures.erase(
            //     std::remove_if(_programFutures.begin(), _programFutures.end(), [this](ProgramFuture &future) {
            //         bool ret = future.valid() && TaskFinished(future);
            //         return ret;
            //     }),
            //     _programFutures.end());
        }
        // catch (...)
        // {
        //     std::cerr << "caught ex [1]" << '\n';
        // }
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
        ReadResult<Shader> result = resMgr.Read<ShaderReader, Shader, ShaderOption>(filename, option);
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
        option->AddSuccessFunc(1, this, &ShaderManager::OnReadShadersSuccess, option);
        return resMgr.ReadAsync<ShaderReader, Shader, ShaderOption>(filename, option);
    }

    ReadResult<ShaderProgram> ShaderManager::ReadShadersAsync(ShaderOption options[], int32 num)
    {
        ResourceManager& resMgr = ResourceMgrInstance();

        bool readyLink = true;
        ShaderProgram::Ptr program = std::make_shared<ShaderProgram>(num);
        Shader::Ptr shader = nullptr;

        std::string programFilename;
        for(int i = 0; i < num; i++)
        {
            auto path = options[i]._optionData.filename;
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

        ShaderProgramOption programOption;
        programOption._program = program;
        resMgr.ReadAsync<ShaderReader, ShaderProgram, ShaderProgramOption>(programFilename.c_str(), &programOption);

        for(int i = 0; i < num; i++)
        {
            options[i]._optionData.program = program;
            ReadShaderAsync(options[i]._optionData.filename.c_str(), &options[i]);
        }

        using Status = ReadResult<ShaderProgram>::Status;
        return ReadResult<ShaderProgram>(nullptr, program, Status::LOADING);
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
        }
    }

    void ShaderManager::OnReadShaderFailed() 
    {}

    void ShaderManager::OnReadShadersSuccess(Object::Ptr obj, ShaderOption* option)
    {
        if(option == nullptr)
        {
            return;
        }

        Shader::Ptr shader = std::dynamic_pointer_cast<Shader>(obj);
        if (shader != nullptr)
        {
            ShaderProgram::Ptr program = option->_optionData.program;
            if(program != nullptr)
            {
                program->AddShader(shader);
                bool linked = program->Link();
            }
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
