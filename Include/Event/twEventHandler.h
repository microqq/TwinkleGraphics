#ifndef TW_EVENTHANDLER_H
#define TW_EVENTHANDLER_H

#include <functional>
#include <vector>
#include <algorithm>
#include <atomic>

#include "twObject.h"
#include "twEventArgs.h"
#include "twConsoleLog.h"

namespace TwinkleGraphics
{
    typedef unsigned int HandlerId;
    class EventHandler : public Object
    {
    public:
        typedef void(*HandlerFuncPointer)(Object::Ptr, BaseEventArgs::Ptr);
        typedef std::function<void(Object::Ptr, BaseEventArgs::Ptr)> HandlerFunc;
        typedef std::shared_ptr<HandlerFunc> HandlerFuncPtr;
        typedef std::shared_ptr<EventHandler> Ptr;

        using HFuncIterator = std::vector<HandlerFuncPtr>::iterator;

        class WrapHandlerFunc
        {
            public:
                WrapHandlerFunc(const HandlerFuncPtr& funcPtr)
                    : _funcPtr(funcPtr)
                {}
                ~WrapHandlerFunc(){}

            private:
                HandlerFuncPtr _funcPtr;;
        };     


        EventHandler()
            : Object()
        {
            _handlerId = ++HandlerIdCounter;
        }

        EventHandler(const HandlerFuncPtr& funcPtr)
            : Object()
        {
            _handlerFuncList.push_back(funcPtr);
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

        EventHandler& operator+=(const HandlerFuncPtr& funcPtr)
        {
            HFuncIterator iter = FindHandlerFunc(funcPtr);
            if(iter != _handlerFuncList.end())
            {
                return *this;
            }

            if(funcPtr != nullptr)
            {
                Add(funcPtr);
            }

            return *this;
        }

        template <class T>
        void AddMemberFunction(const HandlerFunc func)
        {
            HFuncIterator iter = FindMemberHandlerFunc<T>(func);
            if(iter != _handlerFuncList.end())
            {
                return;
            }

            HandlerFuncPtr funcPtr = std::make_shared<HandlerFunc>(func);
            if(funcPtr != nullptr)
            {
                Add(funcPtr);
            }
        }

        EventHandler& operator-=(const HandlerFuncPtr& funcPtr)
        {
            HFuncIterator iter = FindHandlerFunc(funcPtr);
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
        void Add(const HandlerFuncPtr& funcPtr)
        {
            // std::vector<std::function<***>> makes compile error : "no match for 'operator=='"
            // https://stackoverflow.com/questions/18666486/stdvector-of-stdfunctions-find

            _handlerFuncList.push_back(funcPtr);
        }
        void Remove(HFuncIterator iter)
        {
            if(iter != _handlerFuncList.end())
            {
                _handlerFuncList.erase(iter);
            }
        }

        HFuncIterator FindHandlerFunc(const HandlerFunc &func)
        {
            HandlerFuncPointer const* findPointer = func.target<HandlerFuncPointer>();

            HFuncIterator begin = _handlerFuncList.begin();
            HFuncIterator end = _handlerFuncList.end();
            while (begin != end)
            {
                HandlerFuncPointer *pointer = (*begin)->target<HandlerFuncPointer>();
                if (pointer == nullptr)
                {
                    ++begin;
                    continue;
                }

                if (*pointer == *findPointer)
                {
                    return begin;
                }
                ++begin;
            }
            return end;
        }

        HFuncIterator FindHandlerFunc(const HandlerFuncPtr &funcPtr)
        {
            return FindHandlerFunc(*funcPtr);
        }

        template <class T>
        HFuncIterator FindMemberHandlerFunc(const HandlerFunc& func)
        {
            typedef void(T::*FuncType)(Object::Ptr, BaseEventArgs::Ptr);

            // Console::LogInfo("Member function type: ", func.target_type().name(), "\n");
            FuncType const* findPointer = func.template target<FuncType>();

            HFuncIterator begin = _handlerFuncList.begin();            
            HFuncIterator end = _handlerFuncList.end();
            while(begin != end)
            {
                FuncType const* pointer = (*begin)->template target<FuncType>();
                if(pointer == nullptr)
                {
                    ++begin;
                    continue;
                }

                if(*pointer == *findPointer)
                {
                    return begin;
                }
                ++begin;
            }
            return end;
        }

        template <class T>
        HFuncIterator FindMemberHandlerFunc(const HandlerFuncPtr& funcPtr)
        {            
            return FindMemberHandlerFunc<T>(*funcPtr);
        }

    private:
        std::vector<HandlerFuncPtr> _handlerFuncList;
        HandlerId _handlerId;
        static std::atomic_uint HandlerIdCounter;
    };
} // namespace TwinkleGraphics

#endif