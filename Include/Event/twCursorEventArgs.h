#ifndef TW_CURSOREVENTARGS_H
#define TW_CURSOREVENTARGS_H

#include "twInputEventArgs.h"

namespace TwinkleGraphics
{
    class CursorEventArgs : public InputEventArgs
    {
    public:
        typedef std::shared_ptr<CursorEventArgs> Ptr;

        static EventId ID;

        explicit CursorEventArgs()
            : InputEventArgs()
        {
        }
        virtual ~CursorEventArgs() {}

        virtual EventId GetEventId() override
        {
            return CursorEventArgs::ID;
        }

    private:
    };
} // namespace TwinkleGraphics

#endif