
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
    ReadResult<TextSource::Ptr> TextReader::Read(const char *filename, ReaderOption *option)
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

            return ReadResult<TextSource::Ptr>(textSource, ReadResult<TextSource::Ptr>::Status::SUCCESS);
        }

        Console::LogWarning("Text: TextReader open text file ", filename, " failed.\n");

        return ReadResult<TextSource::Ptr>(ReadResult<TextSource::Ptr>::Status::FAILED);
    }

    // void TextReader::ReadAsync(const char *filename, ReaderOption *option)
	// {
		
	// }

    TextSource::Ptr TextManager::ReadText(const char* filename)
    {
        ResourceManagerInst resMgr;
        ReadResult<TextSource::Ptr> result = resMgr->Read<TextReader, TextSource::Ptr>(filename, nullptr);
        TextSource::Ptr text = result.GetSharedObject();

        return text;
    }

} // namespace TwinkleGraphics
