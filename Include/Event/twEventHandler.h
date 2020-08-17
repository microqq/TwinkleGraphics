#ifndef TW_EVENTHANDLER_H
#define TW_EVENTHANDLER_H

#include <functional>
#include <vector>
#include <algorithm>
#include <atomic>

#include "twObject.h"
#include "twEventArgs.h"
#include "twConsoleLog.h"

#define MemFuncTypePart1 typedef void
#define MemFuncTypePart2(T) (T::*T ##MemFuncType)
#define MemFuncTypePart3 (Object::Ptr, BaseEventArgs::Ptr)

#define DefMemFuncType(T) MemFuncTypePart1 MemFuncTypePart2(T)MemFuncTypePart3
#define MemFuncType(T) T##MemFuncType

#define EventHandlerCallBack(CALLBACK, CLASS_NAME, MEMBER_FUNC)              \
    HandlerFuncPointer CALLBACK =                                            \
        [](Object::Ptr sender, BaseEventArgs::Ptr args) {                    \
            CLASS_NAME *listener = dynamic_cast<CLASS_NAME *>(sender.get()); \
            listener->MEMBER_FUNC(sender, args);                             \
        };

#define RegisterEventHandlerCallBack(HANDLER, CALLBACK) \
    {                                                   \
        HandlerFunction func(CALLBACK);                 \
        HANDLER += func;                                \
    }

#define UnRegisterEventHandlerCallBack(HANDLER, CALLBACK) \
    {                                                     \
        HandlerFunction func(CALLBACK);                   \
        HANDLER -= func;                                  \
    }

namespace TwinkleGraphics
{
    typedef unsigned int HandlerId;
    typedef void (*HandlerFuncPointer)(Object::Ptr, BaseEventArgs::Ptr);
    typedef std::function<void(Object::Ptr, BaseEventArgs::Ptr)> HandlerFunction;

    template <class T>
    struct HandlerFunctionWrapper
    {
        typedef void (T::*TFuncType)(Object::Ptr, BaseEventArgs::Ptr) const;

        HandlerFunctionWrapper(T* t, TFuncType func)
            : handlerFunction(std::bind(func, t, std::placeholders::_1, std::placeholders::_2))
        {
        }

        HandlerFunction handlerFunction;
    };

    class EventHandler : public Object
    {
    public:
        typedef std::shared_ptr<HandlerFunction> HandlerFuncPtr;
        typedef std::shared_ptr<EventHandler> Ptr;

        using HFuncIterator = std::vector<HandlerFunction>::iterator;

        EventHandler()
            : Object()
        {
            _handlerId = ++HandlerIdCounter;
        }

        EventHandler(const HandlerFunction& func)
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

        const HandlerFunction& operator[](int index)
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

        template <class T>
        EventHandler& operator+=(const HandlerFunctionWrapper<T>& wrapper)
        {
            typedef void (T::*TFuncType)(Object::Ptr, BaseEventArgs::Ptr) const;
            using BindFunctionType = decltype(std::bind((TFuncType)nullptr, (T*)nullptr, std::placeholders::_1, std::placeholders::_2));

            const HandlerFunction& func = wrapper.handlerFunction;
            HFuncIterator iter = FindHandlerFunc<BindFunctionType>(func);
            if (iter != _handlerFuncList.end())
            {
                return *this;
            }

            Add(func);

            return *this;
        }

        EventHandler& operator+=(const HandlerFunction& func)
        {
            HFuncIterator iter = FindHandlerFunc(func);
            if (iter != _handlerFuncList.end())
            {
                return *this;
            }

            Add(func);

            return *this;
        }

        template <class T>
        EventHandler& operator-=(const HandlerFunctionWrapper<T>& wrapper)
        {
            typedef void (T::*TFuncType)(Object::Ptr, BaseEventArgs::Ptr) const;
            using BindFunctionType = decltype(std::bind((TFuncType)nullptr, (T*)nullptr, std::placeholders::_1, std::placeholders::_2));

            const HandlerFunction& func = wrapper.handlerFunction;
            HFuncIterator iter = FindHandlerFunc<BindFunctionType>(func);
            if (iter != _handlerFuncList.end())
            {
                Remove(iter);
            }

            return *this;
        }


        EventHandler& operator-=(const HandlerFunction& func)
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

        void operator()(Object::Ptr sender, BaseEventArgs::Ptr args)
        {
            Invoke(sender, args);
        }

        inline HandlerId GetHandlerId() const { return _handlerId; }
        inline int GetHandlerFuncSize() { return _handlerFuncList.size(); }

        void FindHandlerFunc(const HandlerFunction& func, bool& finded)
        {
            HFuncIterator iter = FindHandlerFunc(func);
            finded = (iter != _handlerFuncList.end());
        }

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

        void RemoveAll()
        {
            _handlerFuncList.clear();
        }

        void Invoke(Object::Ptr sender, BaseEventArgs::Ptr args)
        {
            for(auto func : _handlerFuncList)
            {
                func(sender, args);
            }
        }

    private:
        void Add(const HandlerFunction& func)
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

        template <class TargetType>
        HFuncIterator FindHandlerFunc(const HandlerFunction &func)
        {
            HFuncIterator begin = _handlerFuncList.begin();
            HFuncIterator end = _handlerFuncList.end();

            const auto &findPointer = func.target<TargetType*>();
            if(findPointer == nullptr)
            {
                return end;
            }

            while (begin != end)
            {
                const auto& pointer = (*begin).target<TargetType*>();
                if (pointer == nullptr)
                {
                    ++begin;
                    continue;
                }

                if (pointer == findPointer)
                {
                    return begin;
                }
                ++begin;
            }
            return end;
        }


        HFuncIterator FindHandlerFunc(const HandlerFunction &func)
        {
            HFuncIterator begin = _handlerFuncList.begin();
            HFuncIterator end = _handlerFuncList.end();

            HandlerFuncPointer const* findPointer = func.target<HandlerFuncPointer>();
            if(findPointer == nullptr)
            {
                return end;
            }

            while (begin != end)
            {
                HandlerFuncPointer *pointer = (*begin).target<HandlerFuncPointer>();
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

        // template <class T>
        // HFuncIterator FindMemberHandlerFunc(const HandlerFunction& func)
        // {
        //     typedef void(T::*FuncType)(Object::Ptr, BaseEventArgs::Ptr);

        //     // Console::LogInfo("Member function type: ", func.target_type().name(), "\n");
        //     FuncType const* findPointer = func.template target<FuncType>();

        //     HFuncIterator begin = _handlerFuncList.begin();            
        //     HFuncIterator end = _handlerFuncList.end();
        //     while(begin != end)
        //     {
        //         FuncType const* pointer = (*begin)->template target<FuncType>();
        //         if(pointer == nullptr)
        //         {
        //             ++begin;
        //             continue;
        //         }

        //         if(*pointer == *findPointer)
        //         {
        //             return begin;
        //         }
        //         ++begin;
        //     }
        //     return end;
        // }

    private:
        // try to traverse std::vector<std::function<***>> find element makes compile error : "no match for 'operator=='"
        // https://stackoverflow.com/questions/18666486/stdvector-of-stdfunctions-find

        std::vector<HandlerFunction> _handlerFuncList;
        HandlerId _handlerId;
        static std::atomic_uint HandlerIdCounter;
    };
} // namespace TwinkleGraphics

#endif