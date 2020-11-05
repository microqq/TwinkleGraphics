#ifndef TW_FOCUSEVENTARGS_H
#define TW_FOCUSEVENTARGS_H

#include "twInputEventArgs.h"

namespace TwinkleGraphics
{
    class FocusEventArgs : public InputEventArgs
    {
    public:
        typedef std::shared_ptr<FocusEventArgs> Ptr;

        static EventId ID;

        explicit FocusEventArgs()
            : InputEventArgs()
        {
        }
        virtual ~FocusEventArgs() {}

        virtual EventId GetEventId() override
        {
            return FocusEventArgs::ID;
        }

    private:
    };
} // namespace TwinkleGraphics

#endif