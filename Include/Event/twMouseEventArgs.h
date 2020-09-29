#ifndef TW_MOUSEEVENTARGS_H
#define TW_MOUSEEVENTARGS_H

#include "twInputEventArgs.h"

namespace TwinkleGraphics
{
    class MouseEventArgs : public InputEventArgs
    {
    public:
        typedef std::shared_ptr<MouseEventArgs> Ptr;

        static EventId ID;

        explicit MouseEventArgs()
            : InputEventArgs()
        {
        }
        virtual ~MouseEventArgs() {}

        virtual EventId GetEventId() override
        {
            return MouseEventArgs::ID;
        }

    private:
        
    };

} // namespace TwinkleGraphics

#endif