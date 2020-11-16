#ifndef TW_SCROLLEVENTARGS_H
#define TW_SCROLLEVENTARGS_H

#include "twInputEventArgs.h"

namespace TwinkleGraphics
{
    class InputManager;

    class ScrollEventArgs : public InputEventArgs
    {
    public:
        typedef std::shared_ptr<ScrollEventArgs> Ptr;

        static EventId ID;

        explicit ScrollEventArgs()
            : InputEventArgs()
        {
        }
        virtual ~ScrollEventArgs() {}

        virtual EventId GetEventId() override
        {
            return ScrollEventArgs::ID;
        }

    private:
        dvec2 _offset;

        friend class InputManager;
    };
} // namespace TwinkleGraphics

#endif