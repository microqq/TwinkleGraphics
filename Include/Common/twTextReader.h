#ifndef TW_TEXTREADER_H
#define TW_TEXTREADER_H

#include "twSingleton.h"
#include "twResource.h"

namespace TwinkleGraphics
{
    class TextManager;
    typedef Singleton<TextManager> TextManagerInst;

    struct TextSource : public SourceHandle
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

        TextReader();
        virtual ~TextReader();

        template <typename TPtr>
        ReadResult<TPtr> Read(const char *filename, ReaderOption *option);

        // virtual void ReadAsync(const char *filename, ReaderOption *option) override;

        DECLARE_READERID;
    };

    class TextManager
    {
    public:
        TextManager() {}
        ~TextManager() {}

        TextSource::Ptr ReadText(const char* filename);

    private:
        std::map<uint32, TextSource::Ptr> _textSources;
    };

} // namespace TwinkleGraphics
#endif
