#ifndef TW_KEYEVENT_H
#define TW_KEYEVENT_H

#include "twEventArgs.h"

namespace TwinkleGraphics
{
    class KeyEventArgs : public BaseEventArgs
    {
    public:
        typedef std::shared_ptr<KeyEventArgs> Ptr;

        static EventId ID;

        explicit KeyEventArgs()
            : BaseEventArgs()
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