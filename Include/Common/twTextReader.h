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
        , public Reference<TextReader>
        , public INonCopyable
    {
    public:
        typedef std::shared_ptr<TextReader> Ptr;

        TextReader();
        TextReader(ReaderOption *option);
        virtual ~TextReader();

        ReadResult<TextSource> Read(const char *filename);
        ReadResult<TextSource> ReadAsync(std::string filename);

        void SetOption(ReaderOption* option) 
        {
            if(option == nullptr)
                return;

            if (_option != nullptr)
            {
                SAFE_DEL(_option);
            }       
            _option = new ReaderOption(*option);
        }
        DECLARE_READERID;
    };

} // namespace TwinkleGraphics
#endif
