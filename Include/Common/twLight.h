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
    using Ptr = std::shared_ptr<Light>;
    
    Light();
    virtual ~Light();


private:
    Light::Ptr _light{nullptr};
};

using LightPtr = Light::Ptr;

} // namespace TwinkleGraphics

#endif