
#include "twTextReader.h"
#include "twConsoleLog.h"

namespace TwinkleGraphics
{
    TextReader::TextReader()
    {
        // INITIALISE_READERID
    }

    TextReader::TextReader(ReaderOption *option)
        : ResourceReader()
    {
        if(option != nullptr)
        {
            if(_option != nullptr)
            {
                SAFE_DEL(_option); 
            }
			_option = new ReaderOption(*option);
        }
    }

    TextReader::~TextReader()
    {
    }
    DEFINE_READERID(TextReader)

    ReadResult<TextSource> TextReader::Read(const char *filename)
    {
        FILE *fp;
        fp = fopen(filename, "rb");
        if (fp)
        {
            Console::LogInfo("TextReader open text file ", filename, " successed.\n");

            //opengl programing guide 8th source code
            //read source
            fseek(fp, 0, SEEK_END);
            int len = ftell(fp);
            fseek(fp, 0, SEEK_SET);

            char *source = new char[len + 1];
            fread(source, 1, len, fp);
            fclose(fp);

            source[len] = 0;

            TextSource::Ptr textSource = std::make_shared<TextSource>();
            textSource->filename = filename;
            textSource->content = source;

            return ReadResult<TextSource>(shared_from_this(), textSource, ReadResult<TextSource>::Status::SUCCESS);
        }

        Console::LogWarning("TextReader open text file ", filename, " failed.\n");

        return ReadResult<TextSource>(ReadResult<TextSource>::Status::FAILED);
    }

    ReadResult<TextSource> TextReader::ReadAsync(std::string filename)
    {
        _asynchronize = true;
        return Read(filename.c_str());
    }
} // namespace TwinkleGraphics
