#ifndef TW_TEXTMANAGER_H
#define TW_TEXTMANAGER_H

#include "twTextReader.h"

namespace TwinkleGraphics
{
    class __SINGLETONExport TextManager : public IUpdatable, public INonCopyable
    {
    public:
        void ReadTextAsync(const char* filename);
        TextSource::Ptr ReadText(const char* filename);
        virtual ~TextManager() {}

        virtual void Update() override {}

    private:
        explicit TextManager() {}

    private:
        std::map<uint32, TextSource::Ptr> _textSources;


        friend class Singleton<TextManager>;
    };

#ifdef __cplusplus
    extern "C"
    {
#endif
        __SINGLETONExport TextManager& TextMgrInstance();
#ifdef __cplusplus
    }
#endif    

    typedef Singleton<TextManager> TextManagerInst;
} // namespace TwinkleGraphics


#endif