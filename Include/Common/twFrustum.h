#ifndef TW_FRUSTUM_H
#define TW_FRUSTUM_H

#include <memory>
#include "twObject.h"

namespace TwinkleGraphics
{
    class Frustum : public Object
    {
    public:
        typedef std::shared_ptr<Frustum> Ptr;

        Frustum() {}
        ~Frustum() {}
    };
} // namespace TwinkleGraphics

#endif