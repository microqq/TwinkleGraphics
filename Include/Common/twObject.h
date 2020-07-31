
#ifndef TW_OBJECT_H
#define TW_OBJECT_H

#include <memory>

#include "twReference.h"

namespace TwinkleGraphics
{
class Object : public Reference<Object>
{
public:
    typedef std::shared_ptr<Object> Ptr;
    typedef std::weak_ptr<Object> WeakPtr;

    Object()
        : Reference<Object>()
    {}
    virtual ~Object() {}
};

} // namespace TwinkleGraphics

#endif