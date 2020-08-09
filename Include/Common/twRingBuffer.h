#ifndef TW_RINGBUFFER_H
#define TW_RINGBUFFER_H

#include <list>

namespace TwinkleGraphics
{
    template <class T>
    class RingBuffer
    {
    public:
        RingBuffer()
        {}
        ~RingBuffer()
        {}

    protected:
        std::list<T> _list;
        int _length;
        int _capacity;
    };
} // namespace TwinkleGraphics

#endif