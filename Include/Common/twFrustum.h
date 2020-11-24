#ifndef TW_FRUSTUM_H
#define TW_FRUSTUM_H

#include <memory>

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