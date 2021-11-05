#ifndef TW_LIGHT_H
#define TW_LIGHT_H

#include "twCommon.h"
#include "twSceneNode.h"

namespace TwinkleGraphics
{
enum class LightType
{
    DIRECTIONAL,
    SPOT,
    POINT
};

class Light : public SceneNode
{
public:
    typedef std::shared_ptr<Light> Ptr;
    
    Light();
    virtual ~Light();


private:
    Light::Ptr _light{nullptr};
};

typedef Light::Ptr LightPtr;

} // namespace TwinkleGraphics

#endif