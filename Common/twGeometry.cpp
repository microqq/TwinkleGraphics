
#include "twGeometry.h"

namespace TwinkleGraphics
{
Geometry::Geometry()
    : Object()
    , _mesh(nullptr)
{
    _transform = std::make_shared<Transform>();
}

Geometry::~Geometry()
{

}

} // namespace TwinkleGraphics
