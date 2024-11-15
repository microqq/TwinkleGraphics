#include "twApplication.h"

namespace TwinkleGraphics {
Application &ApplicationInstance() {
  return Singleton<Application>::Instance();
}

Application::Application() {}

Application::~Application() {}

void Application::Update(float deltaTime) {}

void Application::Destroy() {}

void Application::Init() {}

void Application::Run() {
  if (_mainframe == nullptr) {
    return;
  }

  _mainframe->Initialize();
  EventManager &eventMgrInst = EventMgrInstance();
  while (!_mainframe->CheckIfClose()) {
    _mainframe->PollInputEvents();
    eventMgrInst.Update();

    _mainframe->BeginFrame();
    _mainframe->Update();
    _mainframe->PaintGui();
    _mainframe->EndFrame();
  }

  _mainframe->Destroy();
}

void Application::Quit() {}

} // namespace TwinkleGraphics
