#ifndef TW_MODELMANAGER_H
#define TW_MODELMANAGER_H

#include "twModelReader.h"

namespace TwinkleGraphics
{
    class __TWCOMExport ModelManager : public IUpdatable, public INonCopyable, public IDestroyable
    {
    public:
        virtual ~ModelManager() 
        {
            Destroy();
        }
        virtual void Update() override {} 
        virtual void Destroy() override 
        {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                _futures.clear();
            }
        }

        Model::Ptr ReadModel(const char *filename);
        void ReadModelAsync(const char* filename, ShaderOption* option);

        void AddTaskFuture(std::future<ReadResult<Model>> future);

    private:
        explicit ModelManager()
            : IUpdatable()
            , INonCopyable()
        {}

    private:
        std::vector<std::future<ReadResult<Model>>> _futures;
        std::vector<Model::Ptr> _models;
        std::mutex _mutex;

        friend class Singleton<ModelManager>;
    };    


#ifdef __cplusplus
    extern "C"
    {
#endif
        __TWCOMExport ModelManager& ModelMgrInstance();
#ifdef __cplusplus
    }
#endif

    typedef Singleton<ModelManager> ModelManagerInst;

} // namespace TwinkleGraphics



#endif