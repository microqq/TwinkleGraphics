#ifndef TW_EVENTARGS_H
#define TW_EVENTARGS_H

#include <functional>

#include "twCommon.h"
#include "twObject.h"

namespace TwinkleGraphics
{
    typedef unsigned int EventId;

    class __TWCOMExport BaseEventArgs : public Object
    {
    public:
        typedef std::shared_ptr<BaseEventArgs> Ptr;

        BaseEventArgs() : Object() {}
        virtual ~BaseEventArgs() = 0;

        virtual EventId GetEventId() = 0;

    protected:
    };
} // namespace TwinkleGraphics


#endif