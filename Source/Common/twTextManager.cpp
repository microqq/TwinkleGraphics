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
        resMgr.ReadAsync<TextReader, TextSource>(filename, nullptr);
    }    

    void TextManager::AddTaskFuture(std::future<ReadResult<TextSource>> future)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _futures.emplace_back(std::move(future));
        }
    }


    template <>
    void ResourceManager::PackedReadTask<ReadResult<TextSource>, TextReader>::PushTask()
    {
        // typedef Ret(R::*)(const char*, ReaderOption) Func;
        ResourceManager &resMgr = ResourceMgrInstance();
        if(_asyncRead)
        {
            auto future = resMgr.PushAsyncTask(&TextReader::ReadAsync, _reader, _filename);
            {
                TextManager& textMgr = TextMgrInstance();
                textMgr.AddTaskFuture(std::move(future));
            }
        }
        else
        {
            resMgr.PushTask(&TextReader::Read, _reader, _filename.c_str());
        }
    }    
} // namespace TwinkleGraphics
