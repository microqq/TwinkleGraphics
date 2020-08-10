#ifndef TW_RINGBUFFER_H
#define TW_RINGBUFFER_H

#include <array>

namespace TwinkleGraphics
{
    template <class T, unsigned int CapacityCount = 128U>
    class RingBuffer
    {
    public:
        RingBuffer()
            : _array()
        {}
        ~RingBuffer()
        {}

        inline unsigned int Capacity() { return _capacity; }
        inline unsigned int Length() 
        {
            if(_head == -1 && _tail == 1)
            {
                return 0;
            }

            if(_tail < _head)
            {
                return _capacity - _head + _tail + 1;
            }

            return _tail - _head + 1;
        }

        void PushBack()
        {
            if(Length() == _capacity)
            {
                return;
            }

            if(_head == -1)
            {
                ++_head;
            }

            ++_tail;
            _tail %= _capacity;
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