
#ifndef TW_OBJECT_H
#define TW_OBJECT_H

#include "twReference.hpp"

namespace TwinkleGraphics
{
class Object : Reference
{
public:
    Object()
        : Reference()
    {}
    virtual ~Object() {}
};

} // namespace TwinkleGraphics

#endif