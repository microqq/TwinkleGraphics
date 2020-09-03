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
        virtual ~TextManager() {}

        virtual void Update() override {}

    private:
        explicit TextManager()
            : IUpdatable()
            , INonCopyable()            
        {}

    private:
        std::map<uint32, TextSource::Ptr> _textSources;


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