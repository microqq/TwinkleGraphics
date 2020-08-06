#ifndef TW_EVENT_H
#define TW_EVENT_h

#include "twObject.h"

namespace TwinkleGraphics
{
    typedef unsigned int EventId;

    template <class... Args>
    class Event : public Object
    {
    public:
        typedef std::shared_ptr<Event> Ptr;

        Event()
        {}

        Event(const Event& src)
        {}

        Event(Event&& src)
        {}

        ~Event()
        {}

    private:
        EventId _id;
    };
} // namespace TwinkleGraphics

#endif