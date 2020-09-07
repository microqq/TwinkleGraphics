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

    void ModelManager::ReadModelAsync(const char *filename, ShaderOption *option)
    {
        ResourceManager& resMgr = ResourceMgrInstance();
        resMgr.ReadAsync<ModelReader, Model>(filename, nullptr);
    }


    void ModelManager::AddTaskFuture(std::future<ReadResult<Model>> future)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _futures.emplace_back(std::move(future));
        }
    }    

    template <>
    void ResourceManager::PackedReadTask<ReadResult<Model>, ModelReader>::PushTask()
    {
        // typedef Ret(R::*)(const char*, ReaderOption) Func;
        ResourceManager &resMgr = ResourceMgrInstance();
        auto future = resMgr.PushTask(&ModelReader::ReadAsync, _reader, _filename, _option);
        {
            ModelManager& modelMgr = ModelMgrInstance();
            modelMgr.AddTaskFuture(std::move(future));
        }
    }       
} // namespace TwinkleGraphics
