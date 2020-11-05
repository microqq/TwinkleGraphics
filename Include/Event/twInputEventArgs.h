#ifndef TW_INPUTEVENTARGS_H
#define TW_INPUTEVENTARGS_H

#include "twEventArgs.h"

namespace TwinkleGraphics
{
    class InputEventArgs : public BaseEventArgs
    {
    public:
        typedef std::shared_ptr<InputEventArgs> Ptr;

        explicit InputEventArgs()
            : BaseEventArgs()
        {
        }
        virtual ~InputEventArgs() = 0;
        virtual EventId GetEventId() = 0;

    private:
    };
} // namespace TwinkleGraphics

#endif