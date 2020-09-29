#ifndef TW_RESIZEEVENTARGS_H
#define TW_RESIZEEVENTARGS_H

#include "twInputEventArgs.h"

namespace TwinkleGraphics
{
    class ResizeEventArgs : public InputEventArgs
    {
    public:
        typedef std::shared_ptr<ResizeEventArgs> Ptr;

        static EventId ID;

        explicit ResizeEventArgs()
            : InputEventArgs()
        {
        }
        virtual ~ResizeEventArgs() {}

        virtual EventId GetEventId() override
        {
            return ResizeEventArgs::ID;
        }

    private:
    };} // namespace TwinkleGraphics


#endif