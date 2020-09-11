#include "twModelManager.h"
#include "twResourceManager.h"

namespace TwinkleGraphics
{
    ModelManager& ModelMgrInstance() { return Singleton<ModelManager>::Instance(); }
    
    Model::Ptr ModelManager::ReadModel(const char* filename)
    {
        ResourceManager& resMgr = ResourceMgrInstance();
        ReadResult<Model> result = resMgr.Read<ModelReader, Model>(filename, nullptr);
        Model::Ptr model = result.GetSharedObject();

        return model;
    }

    ReadResult<Model> ModelManager::ReadModelAsync(const char *filename, ShaderOption *option)
    {
        ResourceManager& resMgr = ResourceMgrInstance();
        return resMgr.ReadAsync<ModelReader, Model>(filename, nullptr);
    }


    void ModelManager::AddTaskFuture(std::future<ReadResult<Model>> future)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _futures.emplace_back(std::move(future));
        }
    }    

    void ModelManager::OnReadShaderSuccess(Object::Ptr obj)
    {
        Model *model = dynamic_cast<Model *>(obj.get());
        if (model != nullptr)
        {
        }
    }

    void ModelManager::OnReadShaderFailed() 
    {}    

    template <>
    void ResourceManager::PackedReadTask<ReadResult<Model>, ModelReader>::PushTask()
    {
        // typedef Ret(R::*)(const char*, ReaderOption) Func;
        ResourceManager &resMgr = ResourceMgrInstance();
        if(_asyncRead)
        {
            auto future = resMgr.PushAsyncTask(&ModelReader::ReadAsync, _reader, _filename);
            {
                ModelManager& modelMgr = ModelMgrInstance();
                modelMgr.AddTaskFuture(std::move(future));
            }
        }
        else
        {
            resMgr.PushTask(&ModelReader::Read, _reader, _filename.c_str());
        }
    }       
} // namespace TwinkleGraphics
