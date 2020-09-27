

#include "twTextureManager.h"
#include "twResourceManager.h"
#include "twImageManager.h"

namespace TwinkleGraphics
{
    TextureReader::TextureReader()
    {
        // INITIALISE_READERID
    }

    TextureReader::TextureReader(TextureOption *option)
        : ResourceReader()
    {
        if(option != nullptr)
        {
            if(_option != nullptr)
            {
                SAFE_DEL(_option); 
            }
			_option = new TextureOption(*option);
        }
    }

    TextureReader::~TextureReader()
    {
    }
    DEFINE_READERID(TextureReader)

    ReadResult<Texture> TextureReader::Read(const char *filename)
    {
        Texture::Ptr texture = nullptr;
        TextureOption* option = dynamic_cast<TextureOption*>(_option);
        TextureType texType = option->_textureType;
        switch (texType)
        {
        case TextureType::TEXTURE_1D:
            texture = std::make_shared<Texture1D>(option->_immutable, option->_genMipMap);
            break;
        case TextureType::TEXTURE_1D_ARRAY:
            texture = std::make_shared<Texture1DArray>(option->_immutable, option->_genMipMap);
            break;
        case TextureType::TEXTURE_2D:
            texture = std::make_shared<Texture2D>(option->_immutable, option->_genMipMap);
            break;
        case TextureType::TEXTURE_2D_ARRAY:
            texture = std::make_shared<Texture2DArray>(option->_immutable, option->_genMipMap);
            break;
        case TextureType::TEXTURE_3D:
            texture = std::make_shared<Texture3D>(option->_immutable, option->_genMipMap);
            break;
        case TextureType::TEXTURE_CUBE_MAP:
            texture = std::make_shared<TextureCube>(option->_immutable, option->_genMipMap);
            break;
        case TextureType::TEXTURE_CUBE_MAP_ARRAY:
            texture = std::make_shared<TextureCubeArray>(option->_immutable, option->_genMipMap);
            break;
        case TextureType::TEXTURE_RECTANGLE:
            texture = std::make_shared<TextureRectangle>(option->_immutable);
            break;
        case TextureType::TEXTURE_BUFFER:
            texture = std::make_shared<TextureBuffer>(option->_immutable);
            break;        
        default:
            break;
        }

        ImageManager& imgMgr = ImageMgrInstance();
        std::string texFilename = std::string(filename);
        size_t pos = texFilename.find_first_of(":");
        if(pos != std::string::npos)
        {
            std::string imgFilename = texFilename.substr(pos + 1);
            ImageOption imageOption;
            Image::Ptr image = imgMgr.ReadImage(imgFilename.c_str(), &imageOption);
            if(image != nullptr)
            {
                texture->SetImage(image);
                return ReadResult<Texture>(shared_from_this(), texture, ReadResult<Texture>::Status::SUCCESS);
            }
        }

        return ReadResult<Texture>(ReadResult<Texture>::Status::FAILED);
    }

    ReadResult<Texture> TextureReader::ReadAsync(std::string filename)
    {
        _asynchronize = true;
        return Read(filename.c_str());
    }






    TextureManager& TextureMgrInstance() { return Singleton<TextureManager>::Instance(); }

    Texture::Ptr TextureManager::ReadTexture(const char* filename, TextureOption* option)
    {
        ResourceManager& resMgr = ResourceMgrInstance();
        std::string texFilename = "Texture:" + std::string(filename);
        ReadResult<Texture> result = resMgr.Read<TextureReader, Texture, TextureOption>(texFilename.c_str(), option);
        Texture::Ptr texture = result.GetSharedObject();

        return texture;
    }

    ReadResult<Texture> TextureManager::ReadTextureAsync(const char *filename, TextureOption* option)
    {
        ResourceManager& resMgr = ResourceMgrInstance();
        option->AddSuccessFunc(0, this, &OnReadTextureSuccess);
        option->AddFailedFunc(0, this, &OnReadTextureFailed);
        std::string texFilename = "Texture:";
        texFilename +=  filename;
        auto result = resMgr.ReadAsync<TextureReader, Texture, TextureOption>(texFilename.c_str(), option);

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
            texture->CreateFromImage();
            texture->SetValid(true);
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
