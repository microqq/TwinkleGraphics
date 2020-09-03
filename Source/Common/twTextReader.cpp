
#include "twTextReader.h"
#include "twConsoleLog.h"

namespace TwinkleGraphics
{
    TextReader::TextReader()
    {
        // INITIALISE_READERID
    }

    TextReader::~TextReader()
    {
    }
    DEFINE_READERID(TextReader)

    template <>
    ReadResult<TextSource> TextReader::Read(const char *filename, ReaderOption *option)
    {
        FILE *fp;
        fp = fopen(filename, "rb");
        if (fp)
        {
            Console::LogInfo("Text: TextReader open text file ", filename, " successed.\n");

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

            return ReadResult<TextSource>(textSource, ReadResult<TextSource>::Status::SUCCESS);
        }

        Console::LogWarning("Text: TextReader open text file ", filename, " failed.\n");

        return ReadResult<TextSource>(ReadResult<TextSource>::Status::FAILED);
    }

    ReadResult<TextSource> TextReader::ReadAsync(const char *filename, ReaderOption *option)
    {
        return Read<TextSource>(filename, option);
    }
} // namespace TwinkleGraphics
