
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
        Console::LogGTestInfo("Add OnBaseEvent EventHandler.\n");
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


TEST(EventTests, AddEventHandler)
{
    EventHandler::Ptr handler = std::make_shared<EventHandler>(
        std::make_shared<EventHandler::HandlerFunc>(
            [](Object::Ptr sender, BaseEventArgs::Ptr args) {
                Console::LogGTestInfo("Initialise EventHandler.\n");
            }
        )
    );

    EventHandler::HandlerFuncPtr func = std::make_shared<EventHandler::HandlerFunc>(
        [](Object::Ptr sender, BaseEventArgs::Ptr args) {
            Console::LogGTestInfo("Add Another EventHandler.\n");
        }
    );
    EventHandler &handlerRef = *handler;
    handlerRef += func;
    ASSERT_EQ(handlerRef[1], func);
    handlerRef -= func;
    ASSERT_EQ(handlerRef[1], nullptr);

    SampleListener listner1;

    EventHandler::HandlerFunc baseFunc =
        std::bind(&SampleListener::OnBaseEvent, &listner1, std::placeholders::_1, std::placeholders::_2);
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

    EventHandler handler(
        std::make_shared<EventHandler::HandlerFunc>(
            [](Object::Ptr sender, BaseEventArgs::Ptr args) {
                Console::LogGTestInfo("Initialise EventHandler.\n");
            }
        )
    );

    eventMgrInst->Subscribe(SampleEventArgsA::ID, handler);
    Console::LogGTestInfo("Fire SampleEventArgsA\n");
    eventMgrInst->FireImmediately(nullptr, sampleEventA);

    eventMgrInst->UnSubscribe(SampleEventArgsA::ID, handler);
    Console::LogGTestInfo("Fire SampleEventArgsA\n");
    eventMgrInst->FireImmediately(nullptr, sampleEventA);

    SampleListener listener;
    EventHandler::HandlerFunc func =
        std::bind(&SampleListener::OnBaseEvent, &listener, std::placeholders::_1, std::placeholders::_2);
    handler += std::make_shared<EventHandler::HandlerFunc>(func);

    eventMgrInst->Subscribe(SampleEventArgsA::ID, handler);
    Console::LogGTestInfo("Fire SampleEventArgsA\n");
    eventMgrInst->FireImmediately(nullptr, sampleEventA);

    
}
