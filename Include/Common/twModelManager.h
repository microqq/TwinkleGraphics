#ifndef TW_MODELMANAGER_H
#define TW_MODELMANAGER_H

#include "twModelReader.h"
#include "twReaderManager.h"

namespace TwinkleGraphics
{
    class __TWCOMExport ModelManager : public IUpdatable
        , public IReaderManager
        , public INonCopyable
        , public IDestroyable
    {
    public:
        virtual ~ModelManager() 
        {
            Destroy();
        }
        virtual void Update() override 
        {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                RemoveFutures<Model>(_futures);
            }
        } 
        virtual void Destroy() override 
        {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                _futures.clear();
            }
        }

        Model::Ptr ReadModel(const char *filename);
        ReadResult<Model> ReadModelAsync(const char* filename, ReaderOption* option);

        void AddTaskFuture(std::future<ReadResult<Model>> future);

    private:
        explicit ModelManager()
            : IUpdatable()
            , INonCopyable()
            , _futures()
            , _mutex()
        {}
        void OnReadModelSuccess(Object::Ptr obj);
        void OnReadModelFailed();

    private:
        std::vector<std::future<ReadResult<Model>>> _futures;
        std::mutex _mutex;

        friend class Singleton<ModelManager>;
    };


#ifdef __cplusplus
        __TWCOMExport ModelManager& ModelMgrInstance();
#endif

    typedef Singleton<ModelManager> ModelManagerInst;

} // namespace TwinkleGraphics



#endif