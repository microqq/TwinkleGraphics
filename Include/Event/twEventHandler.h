#ifndef TW_EVENTHANDLER_HPP
#define TW_EVENTHANDLER_HPP

#include <functional>
#include <list>
#include <vector>
#include <algorithm>

#include "twObject.h"
#include "twEventArgs.h"

namespace TwinkleGraphics
{
    class EventHandler : public Object
    {
    public:
        // typedef void(*HandlerFunc)(Object::Ptr, T);
        typedef std::function<void(Object::Ptr, BaseEventArgs::Ptr)> HandlerFunc;
        typedef std::shared_ptr<HandlerFunc> HandlerFuncPtr;
        typedef std::shared_ptr<EventHandler> Ptr;

        EventHandler(const HandlerFuncPtr& func)
            : Object()
        {
            _handlerFuncList.push_back(func);
        }

        EventHandler(const EventHandler &src)
            : Object()
        {
            _handlerFuncList = src._handlerFuncList;
        }

        EventHandler(EventHandler &&src)
            : Object()
        {
            _handlerFuncList = std::move(src._handlerFuncList);
        }

        virtual ~EventHandler()
        {
            _handlerFuncList.clear();
        }

        EventHandler& operator=(const EventHandler& src)
        {
            _handlerFuncList = src._handlerFuncList;

            return *this;
        }

        EventHandler& operator=(EventHandler&& src)
        {
            std::swap(_handlerFuncList, src._handlerFuncList);
            return *this;
        }

        EventHandler& operator+=(const HandlerFuncPtr& func)
        {
            Add(func);
            return *this;
        }

        EventHandler& operator-=(const HandlerFuncPtr& func)
        {
            Remove(func);
            return *this;
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
            // std::list<std::function<***>> makes compile error : "no match for 'operator=='"
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
        std::list<HandlerFuncPtr> _handlerFuncList;
        EventId _eventId;
    };
} // namespace TwinkleGraphics

#endif