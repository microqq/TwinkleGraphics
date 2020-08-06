#ifndef TW_EVENTHANDLER_H
#define TW_EVENTHANDLER_H

#include "twObject.h"

namespace TwinkleGraphics
{
    template <class... Args>
    class EventHandler : public Object
    {
    public:
        typedef std::shared_ptr<EventHandler> Ptr;

        EventHandler()
        {
        }
        EventHandler(const EventHandler &src)
        {
        }
        EventHandler(EventHandler &&src)
        {
        }

        ~EventHandler()
        {
        }

    private:
    };
} // namespace TwinkleGraphics

#endif