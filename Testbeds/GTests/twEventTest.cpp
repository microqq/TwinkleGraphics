
#include <functional>
#include <string>
#include <gtest/gtest.h>

#include "twEventHandler.h"
#include "twEventManager.h"
#include "twConsoleLog.h"

using namespace TwinkleGraphics;


class SampleEventArgs : public BaseEventArgs
{
public:
    typedef std::shared_ptr<SampleEventArgs> Ptr;

    static EventId ID;

    SampleEventArgs()
        : BaseEventArgs()
    {
    }
    virtual ~SampleEventArgs() {}

    virtual EventId GetEventId() override
    {
        return SampleEventArgs::ID;
    }
};

EventId SampleEventArgs::ID = std::hash<std::string>{}("SampleEventArgs");


class SampleEventArgsA : public BaseEventArgs
{
public:
    typedef std::shared_ptr<SampleEventArgsA> Ptr;

    static EventId ID;

    SampleEventArgsA()
        : BaseEventArgs()
    {
    }
    virtual ~SampleEventArgsA() {}

    virtual EventId GetEventId() override
    {
        return SampleEventArgsA::ID;
    }
};

EventId SampleEventArgsA::ID = std::hash<std::string>{}("SampleEventArgsA");



class SampleListener
{
public:
    SampleListener()
        : _handler()
    {
        // EventHandlerFunctionPtr funcOnBaseEvent = std::make_shared<EventHandlerFunction>(
        //     std::bind(&SampleListener::OnBaseEvent
        //         , this
        //         , std::placeholders::_1
        //         , std::placeholders::_2
        //     )
        // );

        EventHandlerFunctionPtr funcOnBaseEvent = MakeClassEventHandlerFunPtr(
            &SampleListener::OnBaseEvent, 
            this
        );

        _handler += funcOnBaseEvent;
        _handler += funcOnBaseEvent;
        _handler += funcOnBaseEvent;

        _handler -= funcOnBaseEvent;

        EventHandlerFunctionPtr funcOnHitEvent = std::make_shared<EventHandlerFunction>(
            std::bind(&SampleListener::OnHitEvent
                , this
                , std::placeholders::_1
                , std::placeholders::_2
            )
        );
        _handler += funcOnHitEvent;

        Subscribe();
        _sequence = counter;
        ++counter;
    }
    ~SampleListener(){}

    void Subscribe()
    {
        EventManagerInst eventMgrInst;
        eventMgrInst->Subscribe(SampleEventArgsA::ID, _handler);
    }

    void UnSubscribe()
    {
        EventManagerInst eventMgrInst;
        eventMgrInst->UnSubscribe(SampleEventArgsA::ID, _handler);
    }

    void OnBaseEvent(Object::Ptr sender, BaseEventArgs::Ptr e) const
    {
        Console::LogGTestInfo("SampleListener Instance ", _sequence, "'s Handler(Id) ", _handler.GetHandlerId(), " Add OnBaseEvent EventHandler.\n");
    }

    void OnHitEvent(Object::Ptr sender, BaseEventArgs::Ptr e) const
    {
        Console::LogGTestInfo("SampleListener Instance ", _sequence, "'s Handler(Id) ", _handler.GetHandlerId(), " Add OnHitEvent EventHandler.\n");
    }

    EventHandler _handler;
    int _sequence = 0;

    static int counter;
};
DefMemEventHandlerType(SampleListener);
int SampleListener::counter = 0;


// en.cppreference.com/w/cpp/utility/functional/function/target.html
void f(Object::Ptr, BaseEventArgs::Ptr) 
{
    Console::LogGTestInfo("Initialise Global f(******) EventHandler.\n");
}
void g(Object::Ptr, BaseEventArgs::Ptr) {  }
void test(std::function<void(Object::Ptr, BaseEventArgs::Ptr)> arg)
{ 
    typedef void (*FPTR)(Object::Ptr, BaseEventArgs::Ptr);
    // void (*const* ptr)(Object::Ptr, BaseEventArgs::Ptr) = 
    //     arg.target<void(*)(Object::Ptr, BaseEventArgs::Ptr)>();

    FPTR* ptr= arg.target<FPTR>();

    if (ptr && *ptr == f)
    {
        Console::LogGTestInfo("it is the function f\n");
    }
    else if (ptr && *ptr == g)
    {
        Console::LogGTestInfo("it is the function g\n");
    }
    else if(ptr)
    {
        Console::LogGTestInfo("it is not nullptr\n");
    }

    if(ptr == nullptr)
    {
        typedef void (SampleListener::*MEMFPTR)(Object::Ptr, BaseEventArgs::Ptr) const;
        MEMFPTR fnPtr = &SampleListener::OnBaseEvent;
        
        using BindFunctionType = decltype(std::bind((MEMFPTR)nullptr, (SampleListener*)nullptr, std::placeholders::_1, std::placeholders::_2));

        auto pptr= arg.target<BindFunctionType>();
        if(pptr != nullptr)
        {
            Console::LogGTestInfo("it is a class member function, and is not nullptr\n");
        }
    }
}


// en.cppreference.com/w/cpp/utility/functional/function.html
struct Foo {
    Foo(int num) : num_(num) {}
    void print_add(int i) const { Console::LogGTestInfo("Foo: ", num_+i, '\n'); }
    int num_;
};

TEST(EventTests, AddEventHandler)
{
    //EventHandler(const EventHandlerFunction& func)
    EventHandler handler(std::make_shared<EventHandlerFunction>(
        [](Object::Ptr sender, BaseEventArgs::Ptr args) {
            Console::LogGTestInfo("Initialise EventHandler.\n");
        })
    );

    //Lambda handler function
    EventHandlerFunctionPtr lambdaPtr = std::make_shared<EventHandlerFunction>(
        [](Object::Ptr sender, BaseEventArgs::Ptr args) {
            Console::LogGTestInfo("Add Lambda Num(1) EventHandler.\n");
        }
    );
    //EventHandler::operator+=
    handler += lambdaPtr;
    ASSERT_EQ(handler[1] != nullptr, true);

    //EventHandler::operator-=
    handler -= lambdaPtr;
    ASSERT_EQ(handler.GetHandlerFuncSize(), 1);

    SampleListener listener;
    EventHandlerFunctionPtr baseFuncPtr = std::make_shared<EventHandlerFunction>(
        std::bind(&SampleListener::OnBaseEvent, &listener
        , std::placeholders::_1
        , std::placeholders::_2)
    );
    handler += baseFuncPtr;

    Object::Ptr objectPtr = std::make_shared<Object>();
    SampleEventArgs::Ptr sampleEvent1 = std::make_shared<SampleEventArgs>();
    SampleEventArgs::Ptr sampleEvent2 = std::make_shared<SampleEventArgs>();
    SampleEventArgsA::Ptr sampleEventA = std::make_shared<SampleEventArgsA>();

    ASSERT_EQ(sampleEvent1->GetEventId() != -1, true);
    ASSERT_EQ(sampleEvent2->GetEventId() != -1, true);
    ASSERT_EQ(sampleEventA->GetEventId() != -1, true);
    ASSERT_EQ(sampleEventA->GetEventId() != sampleEvent1->GetEventId(), true);
    ASSERT_EQ(sampleEvent2->GetEventId() != sampleEvent1->GetEventId(), false);

    Console::LogGTestInfo("SampleEvent Instance 1 EventId: ", sampleEvent1->GetEventId(), "\n");
    Console::LogGTestInfo("SampleEvent Instance 2 EventId: ", sampleEvent2->GetEventId(), "\n");
    Console::LogGTestInfo("SampleEventA EventId: ", sampleEventA->GetEventId(), "\n");

    handler.Invoke(objectPtr, sampleEvent1);
};

TEST(EventTests, FireEvent)
{
    EventManagerInst eventMgrInst;
    SampleEventArgsA::Ptr sampleEventA = std::make_shared<SampleEventArgsA>();

    //EventHandler(const EventHandlerFunction& func)
    EventHandler handler(std::make_shared<EventHandlerFunction>(
        [](Object::Ptr sender, BaseEventArgs::Ptr args) {
            Console::LogGTestInfo("Initialise EventHandler.\n");
        })
    );

    //Lambda handler function
    EventHandlerFunctionPtr lambdaPtr = std::make_shared<EventHandlerFunction>(
        [](Object::Ptr sender, BaseEventArgs::Ptr args) {
            Console::LogGTestInfo("Add Lambda Num(1) EventHandler.\n");
        }
    );

    EventHandlerFunctionPtr lambdaPtr2 = MakeEventHandlerFunPtr(
                [](Object::Ptr sender, BaseEventArgs::Ptr args) {
            Console::LogGTestInfo("Add Lambda Num(2) EventHandler.\n");
        }
    );

    //EventHandler::operator+=
    handler += lambdaPtr;
    ASSERT_EQ(handler[1] != nullptr, true);
    handler += lambdaPtr;
    ASSERT_EQ(handler.GetHandlerFuncSize(), 2);
    handler += lambdaPtr2;

    eventMgrInst->Subscribe(SampleEventArgsA::ID, handler);
    Console::LogGTestInfo("Fire SampleEventArgsA--------1\n");
    eventMgrInst->FireImmediately(nullptr, sampleEventA);

    //EventHandler::operator-=
    handler -= lambdaPtr;
    handler -= lambdaPtr2;
    ASSERT_EQ(handler.GetHandlerFuncSize(), 1);

    // handler updated, so first unsubscribe and then subscribe again
    eventMgrInst->UnSubscribe(SampleEventArgsA::ID, handler);
    eventMgrInst->Subscribe(SampleEventArgsA::ID, handler);
    eventMgrInst->Subscribe(SampleEventArgsA::ID, handler);
    eventMgrInst->Subscribe(SampleEventArgsA::ID, handler);
    Console::LogGTestInfo("Fire SampleEventArgsA--------2\n");
    eventMgrInst->FireImmediately(nullptr, sampleEventA);

    handler -= 0;
    handler += lambdaPtr;
    ASSERT_EQ(handler.GetHandlerFuncSize(), 1);

    eventMgrInst->UnSubscribe(SampleEventArgsA::ID, handler);
    eventMgrInst->UnSubscribe(SampleEventArgsA::ID, handler);
    eventMgrInst->UnSubscribe(SampleEventArgsA::ID, handler);
    eventMgrInst->Subscribe(SampleEventArgsA::ID, handler);
    Console::LogGTestInfo("Fire SampleEventArgsA--------3\n");
    eventMgrInst->FireImmediately(nullptr, sampleEventA);

    //Bind class member function
    SampleListener* listener = new SampleListener;
    SampleListener* listener2 = new SampleListener;
    listener2->UnSubscribe();
    SampleListener* listener3 = new SampleListener;

    eventMgrInst->UnSubscribe(SampleEventArgsA::ID, handler);
    eventMgrInst->Subscribe(SampleEventArgsA::ID, handler);
    eventMgrInst->Subscribe(SampleEventArgsA::ID, handler);
    eventMgrInst->Subscribe(SampleEventArgsA::ID, handler);
    Console::LogGTestInfo("Fire SampleEventArgsA--------4\n");
    eventMgrInst->FireImmediately(nullptr, sampleEventA);

    // en.cppreference.com/w/cpp/utility/functional/function.html
    // std::function<void(const SampleListener&, Object::Ptr, BaseEventArgs::Ptr)> fffFunc = &SampleListener::OnBaseEvent;
    std::function<void(const Foo&, int)> f_add_display = &Foo::print_add;
    const Foo foo(314159);
    f_add_display(foo, 1);
    f_add_display(314159, 1);

    // en.cppreference.com/w/cpp/utility/functional/function/target.html
    test(std::function<void(Object::Ptr, BaseEventArgs::Ptr)>(f));
    test(std::function<void(Object::Ptr, BaseEventArgs::Ptr)>(g));

    SampleListener* listener4 = new SampleListener;
    EventHandlerFunction bindOnBaseEvent = std::bind(&SampleListener::OnBaseEvent
        , listener4
        , std::placeholders::_1
        , std::placeholders::_2
    );

    test(bindOnBaseEvent);
}