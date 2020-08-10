
#include <functional>
#include <string>
#include <gtest/gtest.h>

#include "twEventHandler.h"
#include "twEventManager.h"
#include "twConsoleLog.h"
// #include "twGTestLog.h"

using namespace TwinkleGraphics;

class SampleListener
{
public:
    SampleListener(){}
    ~SampleListener(){}

    void OnBaseEvent(Object::Ptr sender, BaseEventArgs::Ptr e)
    {
        Console::LogWithColor<Console::Color::CYAN>("Add OnBaseEvent EventHandler.\n");
    }
};

class SampleEventArgs : public BaseEventArgs
{
public:
    typedef std::shared_ptr<SampleEventArgs> Ptr;

    SampleEventArgs()
        : BaseEventArgs()
    {
    }
    virtual ~SampleEventArgs() {}
};

class SampleEventArgsA : public BaseEventArgs
{
public:
    typedef std::shared_ptr<SampleEventArgsA> Ptr;

    SampleEventArgsA()
        : BaseEventArgs()
    {
    }
    virtual ~SampleEventArgsA() {}
};


TEST(EventTests, AddEventHandler)
{
    EventHandler::Ptr handler = std::make_shared<EventHandler>(
        std::make_shared<EventHandler::HandlerFunc>(
            [](Object::Ptr sender, BaseEventArgs::Ptr args) {
                Console::LogWithColor<Console::Color::BLUE>("Initialise EventHandler.\n");
            }
        )
    );

    EventHandler::HandlerFuncPtr func = std::make_shared<EventHandler::HandlerFunc>(
        [](Object::Ptr sender, BaseEventArgs::Ptr args) {
            Console::LogWithColor<Console::Color::MAGENTA>("Add Another EventHandler.\n");
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

    Console::LogWithColor<Console::Color::MAGENTA>("SampleEvent Instance 1 EventId: ", sampleEvent1->GetEventId(), "\n");
    Console::LogWithColor<Console::Color::MAGENTA>("SampleEvent Instance 2 EventId: ", sampleEvent2->GetEventId(), "\n");
    Console::LogWithColor<Console::Color::MAGENTA>("SampleEventA EventId: ", sampleEventA->GetEventId(), "\n");

    handler->Invoke(objectPtr, sampleEvent1);
};

TEST(EventTests, FireEvent)
{
    EventHandler::Ptr handler = std::make_shared<EventHandler>(
        std::make_shared<EventHandler::HandlerFunc>(
            [](Object::Ptr sender, BaseEventArgs::Ptr args) {
                Console::LogWithColor<Console::Color::BLUE>("Initialise EventHandler.\n");
            }
        )
    );

    SampleEventArgs::Ptr sampleEvent = std::make_shared<SampleEventArgs>();

    EventManagerInst eventMgrInst;
    eventMgrInst->Fire(100, sampleEvent);

    // BaseEventArgs::Ptr baseEvent = std::make_shared<BaseEventArgs>();
}
