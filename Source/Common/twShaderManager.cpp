
#include "twShaderManager.h"
#include "twResourceManager.h"

namespace TwinkleGraphics
{
    ShaderManager& ShaderMgrInstance() { return Singleton<ShaderManager>::Instance(); }

    ShaderManager::ShaderManager()
        : IUpdatable()
        , INonCopyable()
        , _futures()
        , _shaders()
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
            using ReadStatus = ReadResult<Shader>::Status;
            using Future = std::future<ReadResult<Shader>>;
            
            std::lock_guard<std::mutex> lock(_mutex);

            _futures.erase(
                std::remove_if(_futures.begin(), _futures.end(), [this](Future& future)
                {
                    bool ret = future.valid() && TaskFinished(future);
                    if(ret)
                    {
                        ReadResult<Shader> result = future.get();
                        ResourceReader::Ptr reader = result.GetReader();
                        if(reader != nullptr)
                        {
                            // Console::LogWarning("ShaderReader use count: ", reader.use_count(), " \n");
                            ReaderOption* option = reader->GetReaderOption();
                            if(option!= nullptr)
                            {
                                Shader::Ptr shader = result.GetSharedObject();
                                ReadStatus status = result.GetStatus();
                            
                                if(ReadStatus::SUCCESS == status)
                                {
                                    option->OnReadSuccess(shader);
                                    _shaders.emplace_back(result.GetSharedObject());
                                }
                                else if(ReadStatus::FAILED == status)
                                {
                                    option->OnReadFailed();
                                }
                            }
                        }                        
                    }
                    else //loading or wait load?
                    {}

                    return ret;
                })
                , _futures.end()
            );
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
        _shaders.clear();
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
            Shader::Ptr shader = ReadShader(options[i]._optionData.filename.c_str(), &options[i]);
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
        option->AddSuccessFunc(this, &ShaderManager::OnReadShaderSuccess);
        option->AddFailedFunc(this, &ShaderManager::OnReadShaderFailed);
        resMgr.ReadAsync<ShaderReader, Shader>(filename, option);
    }

    void ShaderManager::ReadShadersAsync(ShaderOption options[], int32 num)
    {
        for(int i = 0; i < num; i++)
        {
            ReadShaderAsync(options[i]._optionData.filename.c_str(), &options[i]);
        }
    }    

    void ShaderManager::AddTaskFuture(std::future<ReadResult<Shader>> future)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _futures.emplace_back(std::move(future));
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

} // namespace TwinkleGraphics
