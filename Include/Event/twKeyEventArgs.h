#ifndef TW_KEYEVENT_H
#define TW_KEYEVENT_H

#include "twInputEventArgs.h"

namespace TwinkleGraphics
{
    class KeyEventArgs : public InputEventArgs
    {
    public:
        typedef std::shared_ptr<KeyEventArgs> Ptr;

        static EventId ID;

        explicit KeyEventArgs()
            : InputEventArgs()
        {
        }
        virtual ~KeyEventArgs() {}

        virtual EventId GetEventId() override
        {
            return KeyEventArgs::ID;
        }

    private:
    };

} // namespace TwinkleGraphics


#endif