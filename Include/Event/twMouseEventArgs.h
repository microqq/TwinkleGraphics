#ifndef TW_MOUSEEVENTARGS_H
#define TW_MOUSEEVENTARGS_H

#include "twEventArgs.h"

namespace TwinkleGraphics
{
    class MouseEventArgs : public BaseEventArgs
    {
    public:
        typedef std::shared_ptr<MouseEventArgs> Ptr;

        static EventId ID;

        explicit MouseEventArgs()
            : BaseEventArgs()
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