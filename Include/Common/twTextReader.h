#ifndef TW_TEXTREADER_H
#define TW_TEXTREADER_H

#include "twSingleton.h"
#include "twResource.h"

namespace TwinkleGraphics
{
    class TextManager;
    typedef Singleton<TextManager> TextManagerInst;

    struct TextReadInfo
    {
        std::string filename;
    };

    struct TextSource
    {
        typedef std::shared_ptr<TextSource> Ptr;
        typedef std::weak_ptr<TextSource> WeakPtr;

        std::string filename;
        std::string content;
    };

    class TextReader : public ResourceReader
    {
    public:
        typedef std::shared_ptr<TextReader> Ptr;

        TextReader(TextReadInfo &read_info);
        virtual ~TextReader();

        template <typename TPtr>
        ReadResult<TPtr> Read(const char *filename, ReaderOption *option);

    private:
        DECLARE_READERID;

        TextReadInfo _readInfo;
    };

    class TextManager
    {
    public:
        TextManager() {}
        ~TextManager() {}

        TextSource::Ptr ReadText(TextReadInfo &readInfo);

    private:
        std::map<uint32, TextSource::Ptr> _textSources;
    };

} // namespace TwinkleGraphics
#endif
