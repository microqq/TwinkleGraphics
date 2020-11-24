
#include "twGeometry.h"

namespace TwinkleGraphics
{
Geometry::Geometry()
    : SceneNode(true)
    , RenderableObject()
    , _mesh(nullptr)
{
}

Geometry::~Geometry()
{

}

} // namespace TwinkleGraphics
