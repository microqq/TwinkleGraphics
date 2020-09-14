

#include "twTextureManager.h"
#include "twResourceManager.h"
#include "twImageManager.h"

namespace TwinkleGraphics
{
    TextureReader::TextureReader()
    {
        // INITIALISE_READERID
    }

    TextureReader::TextureReader(ReaderOption *option)
        : ResourceReader()
    {
        if(option != nullptr)
        {
			_option = new ReaderOption(*option);
        }
    }

    TextureReader::~TextureReader()
    {
    }
    DEFINE_READERID(TextureReader)

    ReadResult<Texture> TextureReader::Read(const char *filename)
    {
        return ReadResult<Texture>(ReadResult<Texture>::Status::FAILED);
    }

    ReadResult<Texture> TextureReader::ReadAsync(std::string filename)
    {
        _asynchronize = true;

        TextureOption* option = dynamic_cast<TextureOption*>(_option);
        Texture::Ptr texture = std::make_shared<Texture>(option->_immutable, option->_genMipMap);

        ImageManager& imgMgr = ImageMgrInstance();
        ImageOption imgOption; 
        imgOption.SetTexture(texture);

        imgMgr.ReadImageAsync(filename.c_str(), &imgOption);
        return ReadResult<Texture>(shared_from_this(), texture, ReadResult<Texture>::Status::SUCCESS);
    }






    TextureManager& TextureMgrInstance() { return Singleton<TextureManager>::Instance(); }

    Texture::Ptr TextureManager::ReadTexture(const char* filename, TextureOption* option)
    {
        ResourceManager& resMgr = ResourceMgrInstance();
        ReadResult<Texture> result = resMgr.Read<TextureReader, Texture, TextureOption>(filename, option);
        Texture::Ptr texture = result.GetSharedObject();

        return texture;
    }

    ReadResult<Texture> TextureManager::ReadTextureAsync(const char *filename, TextureOption* option)
    {
        ResourceManager& resMgr = ResourceMgrInstance();
        auto result = resMgr.ReadAsync<TextureReader, Texture, TextureOption>(filename, option);

        return result;
    }    

    void TextureManager::AddTaskFuture(std::future<ReadResult<Texture>> future)
    {
        {
            std::lock_guard<std::mutex> lock(_mutex);
            _futures.emplace_back(std::move(future));
        }
    }


    void TextureManager::OnReadTextureSuccess(Object::Ptr obj)
    {
        Texture *texture = dynamic_cast<Texture *>(obj.get());
        if (texture != nullptr)
        {
        }
    }

    void TextureManager::OnReadTextureFailed() 
    {}    

    template <>
    void ResourceManager::PackedReadTask<ReadResult<Texture>, TextureReader>::PushTask()
    {
        // typedef Ret(R::*)(const char*, ReaderOption) Func;
        ResourceManager &resMgr = ResourceMgrInstance();
        if(_asyncRead)
        {
            auto future = resMgr.PushAsyncTask(&TextureReader::ReadAsync, _reader, _filename);
            {
                TextureManager& textureMgr = TextureMgrInstance();
                textureMgr.AddTaskFuture(std::move(future));
            }
        }
        else
        {
            resMgr.PushTask(&TextureReader::Read, _reader, _filename.c_str());
        }
    }    
    
} // namespace TwinkleGraphics
