
#include "twTextReader.h"

namespace TwinkleGraphics
{
TextReader::TextReader(TextReadInfo& info)
{

}    

TextReader::~TextReader()
{

}

template <>
ReadResult<TextSource::Ptr> TextReader::Read(const char *filename, ReaderOption *option)
{
    FILE *fp;
    fp = fopen(filename, "rb");
    if (fp)
    {
#ifdef _DEBUG
        std::cout << "Text: TextReader open text file " << filename << " successed" << std::endl;
#endif

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

    return ReadResult<TextSource::Ptr>(ReadResult<TextSource::Ptr>::Status::FAILED);
}

TextSource::Ptr TextManager::ReadText(TextReadInfo &readInfo)
{
    ResourceManagerInst resMgr;
    ReadResult<TextSource::Ptr> result = resMgr->Read<TextReader, TextSource::Ptr>(readInfo.filename.c_str(), nullptr, readInfo);
    TextSource::Ptr text = result.GetSharedObject();

    return text;
}

} // namespace TwinkleGraphics
