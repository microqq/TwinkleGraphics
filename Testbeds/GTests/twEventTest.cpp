
#include <functional>
#include <string>
#include <gtest/gtest.h>

#include "twEventHandler.h"
#include "twEventManager.h"
#include "twConsoleLog.h"

using namespace TwinkleGraphics;

class SampleListener
{
public:

    SampleListener(){}
    ~SampleListener(){}

    void OnBaseEvent(Object::Ptr sender, BaseEventArgs::Ptr e)
    {
        Console::LogGTestInfo("Add SampleListener OnBaseEvent EventHandler.\n");
    }
};

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


// en.cppreference.com/w/cpp/utility/functional/function.html
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
}

TEST(EventTests, AddEventHandler)
{
    //EventHandler(const HandlerFuncPtr& func)
    EventHandler::Ptr handler = std::make_shared<EventHandler>(
        std::make_shared<EventHandler::HandlerFunc>(
            [](Object::Ptr sender, BaseEventArgs::Ptr args) {
                Console::LogGTestInfo("Initialise EventHandler.\n");
            }
        )
    );

    //Lambda handler function
    EventHandler::HandlerFuncPtr funcPtr = std::make_shared<EventHandler::HandlerFunc>(
        [](Object::Ptr sender, BaseEventArgs::Ptr args) {
            Console::LogGTestInfo("Add Lambda EventHandler.\n");
        }
    );
    //EventHandler::operator+=
    EventHandler &handlerRef = *handler;
    // handlerRef += funcPtr;
    // ASSERT_EQ(handlerRef[1] != nullptr, true);

    // //EventHandler::operator-=
    // handlerRef -= funcPtr;
    // ASSERT_EQ(handlerRef[1] == nullptr, true);

    SampleListener listener;
    EventHandler::HandlerFunc baseFunc =
        std::bind(&SampleListener::OnBaseEvent, &listener, std::placeholders::_1, std::placeholders::_2);
    handlerRef += std::make_shared<EventHandler::HandlerFunc>(baseFunc);

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

    handler->Invoke(objectPtr, sampleEvent1);
};

TEST(EventTests, FireEvent)
{
    EventManagerInst eventMgrInst;
    SampleEventArgsA::Ptr sampleEventA = std::make_shared<SampleEventArgsA>();

    //EventHandler(const HandlerFuncPtr& func)
    EventHandler handler(
        std::make_shared<EventHandler::HandlerFunc>(
            [](Object::Ptr sender, BaseEventArgs::Ptr args) {
                Console::LogGTestInfo("Initialise EventHandler.\n");
            }
        )
    );

    EventHandler::HandlerFuncPointer lambda = 
        [](Object::Ptr sender, BaseEventArgs::Ptr args) {
            Console::LogGTestInfo("Add Lambda EventHandler.\n");
    };
    Console::LogGTestInfo("Lambda function address: ", size_t(lambda), "\n");

    //Lambda handler function
    EventHandler::HandlerFuncPtr lambdaFuncPtr = std::make_shared<EventHandler::HandlerFunc>(lambda);
    //EventHandler::operator+=
    handler += lambdaFuncPtr;
    ASSERT_EQ(handler[1] != nullptr, true);
    handler -= lambdaFuncPtr;
    ASSERT_EQ(handler[1] == nullptr, true);
    handler += lambdaFuncPtr;
    ASSERT_EQ(handler[1] != nullptr, true);

    eventMgrInst->Subscribe(SampleEventArgsA::ID, handler);
    Console::LogGTestInfo("Fire SampleEventArgsA--------1\n");
    eventMgrInst->FireImmediately(nullptr, sampleEventA);

    //EventHandler::operator-=
    handler -= lambdaFuncPtr;
    ASSERT_EQ(handler[1], nullptr);

    eventMgrInst->Subscribe(SampleEventArgsA::ID, handler);
    Console::LogGTestInfo("Fire SampleEventArgsA--------2\n");
    eventMgrInst->FireImmediately(nullptr, sampleEventA);

    eventMgrInst->UnSubscribe(SampleEventArgsA::ID, handler);
    Console::LogGTestInfo("Fire SampleEventArgsA--------3\n");
    eventMgrInst->FireImmediately(nullptr, sampleEventA);

    //Bind class member function
    // SampleListener listener;
    // EventHandler::HandlerFunc sampleListenerHFunc = std::bind(&SampleListener::OnBaseEvent, &listener, std::placeholders::_1, std::placeholders::_2);
    // EventHandler::HandlerFuncPtr sampleListenerHFuncPtr = std::make_shared<EventHandler::HandlerFunc>(sampleListenerHFunc);

    //EventHandler::operator+=
    // handler += sampleListenerHFuncPtr;
    // handler += sampleListenerHFuncPtr;
    // handler -= sampleListenerHFuncPtr;
    // handler.AddMemberFunction<SampleListener>(sampleListenerHFunc);
    // handler.AddMemberFunction<SampleListener>(sampleListenerHFunc);

    EventHandler::HandlerFuncPointer lambda2 = 
        [](Object::Ptr sender, BaseEventArgs::Ptr args){
            SampleListener* listener = dynamic_cast<SampleListener*>(sender.get());
            listener->OnBaseEvent(sender, args);
    };
    EventHandler::HandlerFuncPtr lambda2FuncPtr = std::make_shared<EventHandler::HandlerFunc>(lambda2);
    //EventHandler::operator+=
    handler += lambda2FuncPtr;
    handler += lambda2FuncPtr;
    handler += lambda2FuncPtr;
    handler -= lambda2FuncPtr;

    EventHandler::HandlerFunc globalF(f);
    handler += std::make_shared<EventHandler::HandlerFunc>(globalF);
    handler += std::make_shared<EventHandler::HandlerFunc>(globalF);
    handler += std::make_shared<EventHandler::HandlerFunc>(globalF);

    eventMgrInst->Subscribe(SampleEventArgsA::ID, handler);
    Console::LogGTestInfo("Fire SampleEventArgsA--------4\n");
    eventMgrInst->FireImmediately(nullptr, sampleEventA);

    test(std::function<void(Object::Ptr, BaseEventArgs::Ptr)>(f));
    test(std::function<void(Object::Ptr, BaseEventArgs::Ptr)>(g));
    // test(std::function<void(Object::Ptr, BaseEventArgs::Ptr)>(func));
}