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
        // typedef void(*HandlerFunc)(Object::Ptr, T);
        typedef std::function<void(Object::Ptr, BaseEventArgs::Ptr)> HandlerFunc;
        typedef std::shared_ptr<HandlerFunc> HandlerFuncPtr;
        typedef std::shared_ptr<EventHandler> Ptr;

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
            if(func != nullptr)
            {
                Add(func);
            }

            return *this;
        }

        EventHandler& operator-=(const HandlerFuncPtr& func)
        {
            if(func != nullptr)
            {
                Remove(func);
            }
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
            auto iter = std::find(_handlerFuncList.begin(), _handlerFuncList.end(), func);
            if(iter != _handlerFuncList.end())
            {
                return;                
            }

            _handlerFuncList.push_back(func);
        }
        void Remove(const HandlerFuncPtr& func)
        {
            auto iter = std::find(_handlerFuncList.begin(), _handlerFuncList.end(), func);
            if(iter != _handlerFuncList.end())
            {
                _handlerFuncList.erase(iter);
            }
        }

    private:
        std::vector<HandlerFuncPtr> _handlerFuncList;
        HandlerId _handlerId;
        static std::atomic_uint HandlerIdCounter;
    };
} // namespace TwinkleGraphics

#endif