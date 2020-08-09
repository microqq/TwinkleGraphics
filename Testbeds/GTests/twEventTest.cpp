
#include <gtest/gtest.h>
#include "twEventHandler.h"
#include "twEventManager.h"
#include "twConsoleLog.h"

using namespace TwinkleGraphics;

class Listener1
{
public:
    Listener1(){}
    ~Listener1(){}
    void OnBaseEvent(Object::Ptr sender, BaseEventArgs::Ptr e)
    {
        Console::LogWithColor<Console::Color::CYAN>("Add OnBaseEvent EventHandler.\n");
    }
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
    // handlerRef -= func;

    Listener1 listner1;

    EventHandler::HandlerFunc baseFunc =
        std::bind(&Listener1::OnBaseEvent, &listner1, std::placeholders::_1, std::placeholders::_2);
    handlerRef += std::make_shared<EventHandler::HandlerFunc>(baseFunc);

    Object::Ptr objectPtr = std::make_shared<Object>();
    BaseEventArgs::Ptr baseEvent = std::make_shared<BaseEventArgs>();
    handler->Invoke(objectPtr, baseEvent);
};

class SampleEventArgs : public BaseEventArgs
{
public:
    typedef std::shared_ptr<SampleEventArgs> Ptr;
    SampleEventArgs() {}
    virtual ~SampleEventArgs() {}


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
}


int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}


