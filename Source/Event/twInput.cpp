#include "twInput.h"
#include "twMouseEventArgs.h"
#include "twResizeEventArgs.h"


namespace TwinkleGraphics {
InputManager &InputMgrInstance() { return Singleton<InputManager>::Instance(); }

InputManager::InputManager()
    : IUpdatable(), INonCopyable(), _pressedKeys{KeyState::KEY_RELEASE},
      _pressedMouseButtons{MouseState::MOUSE_RELEASE} {}

InputManager::~InputManager() {}

void InputManager::Update(float deltaTime) {}

KeyState InputManager::GetPressedKey(KeyCode key) {
  return _pressedKeys[(int)key];
}

MouseState InputManager::GetPressedMouse(MouseButton button) {
  return _pressedMouseButtons[(int)button];
}

void InputManager::SetPressedKey(KeyCode key, KeyState state) {
  _pressedKeys[(int)key] = state;

  // fire keyevent
  EventManager &eventMgrInst = EventMgrInstance();
}

void InputManager::SetPressedMouse(MouseButton button, MouseState state) {
  _pressedMouseButtons[(int)button] = state;

  // fire mousebuttonevent
  EventManager &eventMgrInst = EventMgrInstance();
}

void InputManager::SetMousePosition(vec2 pos) { _mousePosition = pos; }

void InputManager::SetMousePosition(float x, float y) {
  SetMousePosition(vec2(x, y));
}

void InputManager::SetMouseMove(vec2 pos) {
  _mousePosition = pos;

  // fire mouse move event
  EventManager &eventMgrInst = EventMgrInstance();
  MouseEventArgs::Ptr event = std::make_shared<MouseEventArgs>();
  eventMgrInst.Fire(nullptr, event);
}

void InputManager::SetCursorEnter(vec2 pos, bool entered) {
  _mousePosition = pos;
  _cursorEntered = entered;
}

void InputManager::SetScroll(double dx, double dy) {
  EventManager &eventMgrInst = EventMgrInstance();
}

void InputManager::SetWindowResize(vec2 size) {
  EventManager &eventMgrInst = EventMgrInstance();

  // if(_pressedMouseButtons[MOUSE_BUTTON_LEFT] == MOUSE_RELEASE)
  {
    ResizeEventArgs::Ptr event = std::make_shared<ResizeEventArgs>();
    event->SetSize(size);
    eventMgrInst.Fire(nullptr, event);

    // Console::LogInfo("Window Size X:", size.x, " Y:", size.y, "\n");
  }
}

void InputManager::ReleasePressedMouseButtons() {
  for (auto &key : _pressedKeys) {
    key = KeyState::KEY_RELEASE;
  }
}

void InputManager::ReleasePressedKeys() {
  for (auto &button : _pressedMouseButtons) {
    button = MouseState::MOUSE_RELEASE;
  }
}

void InputManager::Release() {
  ReleasePressedKeys();
  ReleasePressedMouseButtons();
}

} // namespace TwinkleGraphics
