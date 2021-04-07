#ifndef TW_RINGBUFFER_H
#define TW_RINGBUFFER_H

#include <assert.h>
#include <array>
// #include "twConsoleLog.h"

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

        inline T& Head() { return _array[_head]; }
        inline T& Tail() { return _array[_tail]; }

        inline int HeadIndex() { return _head; }
        inline int TailIndex() { return _tail; }

        template<class... Args>
        T* PushBack(Args... args)
        {
            if(Length() == _capacity)
            {
                // Console::LogWarning("RingBuffer There is no free space push back.\n");
                return nullptr;
            }

            if(_head == -1)
            {
                ++_head;
            }

            ++_tail;
            _tail %= _capacity;

            T* ele = new (&(_array[_tail]))T(std::forward<Args>(args)...);
            return ele;
        }

        T* PopFront()
        {
            int length = Length();
            int retIndex = _head;
            if(length == 0)
            {
                // Console::LogWarning("RingBuffer There is no more elements pop front.\n");
                return nullptr;
            }
            else if(1 == length)
            {
                _head = _tail = -1;
            }
            else
            {
                ++_head;
                _head %= _capacity;
            }

            return &(_array[retIndex]);
        }

        T& operator[](unsigned int index)
        {
            int length = Length();
            assert(index >= 0 && index < length);

            int realIndex = (_head + index) % _capacity;
            return _array[realIndex];
        }

    protected:
        std::array<T, CapacityCount> _array;
        unsigned int _capacity = CapacityCount;
        int _head = -1;
        int _tail = -1;
    };
} // namespace TwinkleGraphics

#endif