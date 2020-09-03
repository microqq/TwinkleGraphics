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
        auto future = resMgr.ReadAsync<ModelReader, Model>(filename, nullptr);
    }
} // namespace TwinkleGraphics
