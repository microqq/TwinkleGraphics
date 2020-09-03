#ifndef TW_IMAGEMANAGER_H
#define TW_IMAGEMANAGER_H

#include "twImage.h"

namespace TwinkleGraphics
{
class __COMSINGLETONExport ImageManager : public IUpdatable, public INonCopyable
{
public:
    virtual ~ImageManager();
    virtual void Update() override {}

    Image::Ptr ReadImage(const char* filename);
    void ReadImageAsync(const char* filename);


private:
    explicit ImageManager();
    
    friend class Singleton<ImageManager>;
};
    
#ifdef __cplusplus
    extern "C"
    {
#endif
        __COMSINGLETONExport ImageManager& ImageMgrInstance();
#ifdef __cplusplus
    }
#endif

typedef Singleton<ImageManager> ImageManagerInst;
    
} // namespace TwinkleGraphics



#endif