#ifndef TW_TEXTREADER_H
#define TW_TEXTREADER_H

#include "twSingleton.h"
#include "twResource.h"

namespace TwinkleGraphics
{
    struct TextSource : public Object
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

        template <typename T>
        ReadResult<T> Read(const char *filename, ReaderOption *option);

        ReadResult<TextSource> ReadAsync(const char *filename, ReaderOption *option);

        DECLARE_READERID;
    };

} // namespace TwinkleGraphics
#endif
