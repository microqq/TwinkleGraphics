#include "twMaterial.h"

namespace TwinkleGraphics
{
Material::Material()
    : Object()
{

}

Material::~Material()
{
    
}

RenderPass::RenderPass()
    : Object()
    , _shader(nullptr)
{
}

RenderPass::~RenderPass()
{
}

} // namespace TwinkleGraphics
