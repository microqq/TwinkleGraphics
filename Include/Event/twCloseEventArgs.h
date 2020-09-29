#ifndef TW_CLOSEEVENTARGS_H
#define TW_CLOSEEVENTARGS_H

#include "twInputEventArgs.h"

namespace TwinkleGraphics
{
    class CloseEventArgs : public InputEventArgs
    {
    public:
        typedef std::shared_ptr<CloseEventArgs> Ptr;

        static EventId ID;

        explicit CloseEventArgs()
            : InputEventArgs()
        {
        }
        virtual ~CloseEventArgs() {}

        virtual EventId GetEventId() override
        {
            return CloseEventArgs::ID;
        }

    private:
    };} // namespace TwinkleGraphics


#endif