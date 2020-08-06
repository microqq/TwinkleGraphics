#ifndef TW_RINGBUFFER_H
#define TW_RINGBUFFER_H

#include <list>

namespace TwinkleGraphics
{
    template<class T>
    class RingBuffer
    {
        public:
            RingBuffer();
            ~RingBuffer();

        private:
            std::list<T> _contents;
    };
} // namespace TwinkleGraphics


#endif