#ifndef TW_EVENTHANDLER_H
#define TW_EVENTHANDLER_H

#include <functional>
#include <vector>
#include <algorithm>
#include <atomic>

#include "twObject.h"
#include "twEventArgs.h"
#include "twConsoleLog.h"

#define MemEventHandlerPart1 typedef void
#define MemEventHandlerPart2(T) (T::*T ##MemFuncType)
#define MemEventHandlerPart3 (Object::Ptr, BaseEventArgs::Ptr)

#define DefMemEventHandlerType(T) MemEventHandlerPart1 MemEventHandlerPart2(T)MemEventHandlerPart3
#define MemEventHandlerType(T) T##MemFuncType

namespace TwinkleGraphics
{
    typedef unsigned int HandlerId;
    typedef void (*EventHandlerFuncPointer)(Object::Ptr, BaseEventArgs::Ptr);
    typedef std::function<void(Object::Ptr, BaseEventArgs::Ptr)> EventHandlerFunction;
    typedef std::shared_ptr<EventHandlerFunction> EventHandlerFunctionPtr;

    class EventHandler : public Object
    {
    public:
        typedef std::shared_ptr<EventHandler> Ptr;

        using HFuncIterator = std::vector<EventHandlerFunctionPtr>::iterator;

        EventHandler()
            : Object()
        {
            _handlerId = ++HandlerIdCounter;
        }

        EventHandler(const EventHandlerFunctionPtr& func)
            : Object()
        {
            _handlerFuncList.push_back(func);
            _handlerId = ++HandlerIdCounter;
        }

        EventHandler(const EventHandler &src)
            : Object()
        {
            _handlerFuncList = src._handlerFuncList;
            _handlerId = src._handlerId;
        }

        EventHandler(EventHandler &&src)
            : Object()
        {
            _handlerFuncList = std::move(src._handlerFuncList);
            _handlerId = src._handlerId;
        }

        virtual ~EventHandler()
        {
            _handlerFuncList.clear();
        }

        const EventHandlerFunctionPtr& operator[](int index)
        {
            int size = _handlerFuncList.size();
            assert(index >= 0 && index < size);

            return _handlerFuncList[index];
        }

        EventHandler& operator=(const EventHandler& src)
        {
            _handlerFuncList = src._handlerFuncList;
            _handlerId = src._handlerId;

            return *this;
        }

        EventHandler& operator=(EventHandler&& src)
        {
            std::swap(_handlerFuncList, src._handlerFuncList);
            _handlerId = src._handlerId;

            return *this;
        }

        EventHandler& operator+=(const EventHandlerFunctionPtr& func)
        {
            HFuncIterator iter = FindHandlerFunc(func);
            if (iter != _handlerFuncList.end())
            {
                return *this;
            }

            Add(func);

            return *this;
        }

        EventHandler& operator-=(const EventHandlerFunctionPtr& func)
        {
            HFuncIterator iter = FindHandlerFunc(func);
            if(iter != _handlerFuncList.end())
            {
                Remove(iter);
            }

            return *this;
        }

        EventHandler &operator-=(int index)
        {
            Remove(index);

            return *this;
        }

        bool operator==(const EventHandler& other)
        {
            return _handlerId == other._handlerId;
        }

        void operator()(Object::Ptr sender, BaseEventArgs::Ptr args)
        {
            Invoke(sender, args);
        }

        inline HandlerId GetHandlerId() const { return _handlerId; }
        inline int GetHandlerFuncSize() { return _handlerFuncList.size(); }

        void Remove(int index)
        {
            int size = _handlerFuncList.size();
            assert(index >= 0 && index < size);

            HFuncIterator iter = _handlerFuncList.begin();
            HFuncIterator end = _handlerFuncList.end();
            iter += index;

            if(iter != end)
            {
                _handlerFuncList.erase(iter);
            }
        }

        void Clear()
        {
            _handlerFuncList.clear();
        }

        void Invoke(Object::Ptr sender, BaseEventArgs::Ptr args)
        {
            for(auto func : _handlerFuncList)
            {
                (*func)(sender, args);
            }
        }

        HFuncIterator FindHandlerFunc(const EventHandlerFunctionPtr &func)
        {
            HFuncIterator begin = _handlerFuncList.begin();
            HFuncIterator end = _handlerFuncList.end();

            if(func == nullptr)
            {
                return end;
            }

            while (begin != end)
            {
                if (*begin == nullptr)
                {
                    ++begin;
                    continue;
                }

                EventHandlerFunctionPtr pointer = *begin;
                if (pointer == func)
                {
                    return begin;
                }
                ++begin;
            }
            return end;
        }

    private:
        void Add(const EventHandlerFunctionPtr& func)
        {
            _handlerFuncList.push_back(func);
        }
        void Remove(HFuncIterator iter)
        {
            if(iter != _handlerFuncList.end())
            {
                _handlerFuncList.erase(iter);
            }
        }

    private:
        // try to traverse std::vector<std::function<***>> find element makes compile error : "no match for 'operator=='"
        // https://stackoverflow.com/questions/18666486/stdvector-of-stdfunctions-find

        std::vector<EventHandlerFunctionPtr> _handlerFuncList;
        HandlerId _handlerId;
        static std::atomic_uint HandlerIdCounter;
    };
} // namespace TwinkleGraphics

#endif