#include "twApplication.h"

namespace TwinkleGraphics
{
    Application& ApplicationInstance() { return Singleton<Application>::Instance(); }

    Application::Application()
    {}

    Application::~Application()
    {}

    void Application::Update(float deltaTime)
    {}

    void Application::Destroy()
    {}

    void Application::Init()
    {}

    void Application::Run()
    {
        if(_mainframe == nullptr)
        {
            return;
        }

        EventManager& eventMgrInst = EventMgrInstance();

        while(!_mainframe->CheckClose())
        {
            _mainframe->PollInputEvents();
            eventMgrInst.Update();

            _mainframe->BeginFrame();
            {
                _mainframe->Update();
                _mainframe->PaintGui();
            }
            _mainframe->EndFrame();
        }
    }

    void Application::Quit()
    {}

} // namespace TwinkleGraphics
