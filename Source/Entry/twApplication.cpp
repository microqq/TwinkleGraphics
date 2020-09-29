#include "twApplication.h"

namespace TwinkleGraphics
{
    Application& ApplicationInstance() { return Singleton<Application>::Instance(); }

    Application::Application()
    {}

    Application::~Application()
    {}

    void Application::Update()
    {}

    void Application::Destroy()
    {}

    void Application::Init()
    {}

    void Application::Run()
    {}

    void Application::Quit()
    {}

} // namespace TwinkleGraphics
