#ifndef TW_TEXTMANAGER_H
#define TW_TEXTMANAGER_H

#include "twTextReader.h"

namespace TwinkleGraphics
{
    class __COMSINGLETONExport TextManager : public IUpdatable, public INonCopyable
    {
    public:
        void ReadTextAsync(const char* filename);
        TextSource::Ptr ReadText(const char* filename);
        virtual ~TextManager() 
        {
            {
                std::lock_guard<std::mutex> lock(_mutex);
                _futures.clear();
            }
        }

        virtual void Update() override {}

        void AddTaskFuture(std::future<ReadResult<TextSource>> future);
    private:
        explicit TextManager()
            : IUpdatable()
            , INonCopyable()            
        {}

    private:
        std::vector<std::future<ReadResult<TextSource>>> _futures;
        std::vector<TextSource::Ptr> _texts;
        std::mutex _mutex;

        friend class Singleton<TextManager>;
    };

#ifdef __cplusplus
    extern "C"
    {
#endif
        __COMSINGLETONExport TextManager& TextMgrInstance();
#ifdef __cplusplus
    }
#endif    

    typedef Singleton<TextManager> TextManagerInst;
} // namespace TwinkleGraphics


#endif