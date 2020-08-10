#ifndef TW_RINGBUFFER_H
#define TW_RINGBUFFER_H

#include <array>
#include "twConsoleLog.h"

namespace TwinkleGraphics
{
    template <class T, unsigned int CapacityCount = 128U>
    class RingBuffer
    {
    public:
        RingBuffer()
            : _array()
            , _head(-1)
            , _tail(-1)
        {}
        ~RingBuffer()
        {}

        inline unsigned int Capacity() { return _capacity; }
        inline unsigned int Length() 
        {
            if(_head == -1 && _tail == -1)
            {
                return 0;
            }

            if(_tail < _head)
            {
                return _capacity - _head + _tail + 1;
            }

            return _tail - _head + 1;
        }

        T* PushBack()
        {
            if(Length() == _capacity)
            {
                Console::LogWarning("RingBuffer: There is no free space push back.\n");
                return nullptr;
            }

            if(_head == -1)
            {
                ++_head;
            }

            ++_tail;
            _tail %= _capacity;

            T* ele = new (&(_array[_tail]))T();
            return ele;
        }

        void PopFront()
        {
            if(1 == Length())
            {
                _head = _tail = -1;                
                return;
            }

            ++_head;
            _head %= _capacity;
        }

        T& operator[](unsigned int index)
        {

            return _array[index];
        }

    protected:
        std::array<T, CapacityCount> _array;
        unsigned int _capacity = CapacityCount;
        int _head = -1;
        int _tail = -1;
    };
} // namespace TwinkleGraphics

#endif