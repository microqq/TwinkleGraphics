#ifndef TW_EVENTHANDLER_H
#define TW_EVENTHANDLER_H

#include <functional>
#include <vector>
#include <algorithm>
#include <atomic>

#include "twObject.h"
#include "twEventArgs.h"

namespace TwinkleGraphics
{
    typedef unsigned int HandlerId;
    class EventHandler : public Object
    {
    public:
        typedef void(HandlerFuncPointer)(Object::Ptr, BaseEventArgs::Ptr);
        typedef std::function<void(Object::Ptr, BaseEventArgs::Ptr)> HandlerFunc;
        typedef std::shared_ptr<HandlerFunc> HandlerFuncPtr;
        typedef std::shared_ptr<EventHandler> Ptr;

        using HFuncIterator = std::vector<HandlerFuncPtr>::iterator;

        EventHandler()
            : Object()
        {
            _handlerId = ++HandlerIdCounter;
        }

        EventHandler(const HandlerFuncPtr& func)
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

        inline HandlerId GetHandlerId() { return _handlerId; }

        HandlerFuncPtr operator[](int index)
        {
            if(index < 0 || index >= _handlerFuncList.size())
            {
                return nullptr;
            }

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

        EventHandler& operator+=(const HandlerFuncPtr& func)
        {
            HFuncIterator iter = FindHandlerFunc(func);
            if(iter != _handlerFuncList.end())
            {
                return *this;
            }

            if(func != nullptr)
            {
                Add(func);
            }

            return *this;
        }

        EventHandler& operator-=(const HandlerFuncPtr& func)
        {
            HFuncIterator iter = FindHandlerFunc(func);
            if(iter != _handlerFuncList.end())
            {
                Remove(iter);
            }

            return *this;
        }

        bool operator==(const EventHandler& other)
        {
            return _handlerId == other._handlerId;
        }

        void FindHandlerFunc(const HandlerFunc& func, bool& finded)
        {
            HFuncIterator iter = FindHandlerFunc(func);
            finded = (iter != _handlerFuncList.end());
        }

        void operator()(Object::Ptr sender, BaseEventArgs::Ptr args)
        {
            Invoke(sender, args);
        }

        void Invoke(Object::Ptr sender, BaseEventArgs::Ptr args)
        {
            for(auto func : _handlerFuncList)
            {
                (*func)(sender, args);
            }
        }

    private:
        void Add(const HandlerFuncPtr& func)
        {
            // std::vector<std::function<***>> makes compile error : "no match for 'operator=='"
            // https://stackoverflow.com/questions/18666486/stdvector-of-stdfunctions-find

            _handlerFuncList.push_back(func);
        }
        void Remove(HFuncIterator iter)
        {
            if(iter != _handlerFuncList.end())
            {
                _handlerFuncList.erase(iter);
            }
        }

        HFuncIterator FindHandlerFunc(const HandlerFunc& func)
        {
            HFuncIterator iter = std::find_if(_handlerFuncList.begin(), 
                _handlerFuncList.end(),
                [func](const HandlerFuncPtr& fPtr)
            {
                HandlerFuncPointer** lPointer = (*fPtr).target<HandlerFuncPointer*>();
                HandlerFuncPointer*const* rPointer = func.target<HandlerFuncPointer*>();

                if(lPointer == nullptr || rPointer == nullptr)
                {
                    return false;
                }

                return *lPointer == *rPointer;
            });

            if(iter != _handlerFuncList.end())
            {
                return iter;
            }

            return _handlerFuncList.end();
        }

        HFuncIterator FindHandlerFunc(const HandlerFuncPtr& funcPtr)
        {
            HFuncIterator iter = std::find_if(_handlerFuncList.begin(), 
                _handlerFuncList.end(),
                [funcPtr](const HandlerFuncPtr& fPtr)
            {
                HandlerFuncPointer** lPointer = (*fPtr).target<HandlerFuncPointer*>();
                HandlerFuncPointer** rPointer = (*funcPtr).target<HandlerFuncPointer*>();

                if(lPointer == nullptr || rPointer == nullptr)
                {
                    return false;
                }

                return *lPointer == *rPointer;
            });

            if(iter != _handlerFuncList.end())
            {
                return iter;
            }

            return _handlerFuncList.end();
        }

    private:
        std::vector<HandlerFuncPtr> _handlerFuncList;
        HandlerId _handlerId;
        static std::atomic_uint HandlerIdCounter;
    };
} // namespace TwinkleGraphics

#endif