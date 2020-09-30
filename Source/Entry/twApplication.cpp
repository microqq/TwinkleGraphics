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

        while(!_mainframe->CheckClose())
        {
            _mainframe->DispatchInputEvents();
            _mainframe->BeginFrame();
            {

            }
            _mainframe->EndFrame();
        }
    }

    void Application::Quit()
    {}

} // namespace TwinkleGraphics
