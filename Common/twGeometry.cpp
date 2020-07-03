
#include "twGeometry.h"

namespace TwinkleGraphics
{
Geometry::Geometry()
    : Object()
    , ISceneNode(true)
    , IRenderableObject()
    , _mesh(nullptr)
{
}

Geometry::~Geometry()
{

}

} // namespace TwinkleGraphics
