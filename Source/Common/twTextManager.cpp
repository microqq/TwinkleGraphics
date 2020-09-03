#include "twTextManager.h"
#include "twResourceManager.h"

namespace TwinkleGraphics
{
    TextManager& TextMgrInstance() { return Singleton<TextManager>::Instance(); }

    TextSource::Ptr TextManager::ReadText(const char* filename)
    {
        ResourceManager& resMgr = ResourceMgrInstance();
        ReadResult<TextSource> result = resMgr.Read<TextReader, TextSource>(filename, nullptr);
        TextSource::Ptr text = result.GetSharedObject();

        return text;
    }

    void TextManager::ReadTextAsync(const char *filename)
    {
        ResourceManager& resMgr = ResourceMgrInstance();
        auto future = resMgr.ReadAsync<TextReader, TextSource>(filename, nullptr);
    }    
} // namespace TwinkleGraphics
