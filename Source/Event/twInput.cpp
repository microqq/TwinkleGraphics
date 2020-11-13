#include "twInput.h"

namespace TwinkleGraphics
{
    InputManager &InputMgrInstance()
    {
        return Singleton<InputManager>::Instance();
    }

    InputManager::InputManager()
        : IUpdatable()
        , INonCopyable()
        , _pressedKeys{KeyState::KEY_RELEASE}
        , _pressedMouseButtons{MouseState::MOUSE_RELEASE}
    {
    }

    InputManager::~InputManager()
    {
    }

    void InputManager::Update(float deltaTime)
    {
    }

    KeyState InputManager::GetPressedKey(KeyCode key) 
    {
        return _pressedKeys[(int)key];
    }

    MouseState InputManager::GetPressedMouse(MouseButton button) 
    {
        return _pressedMouseButtons[(int)button];
    }

    void InputManager::SetPressedKey(KeyCode key, KeyState state) 
    {
        _pressedKeys[(int)key] = state;

        // fire keyevent
        EventManager& eventMgrInst = EventMgrInstance();

    }

    void InputManager::SetPressedMouse(MouseButton button, MouseState state) 
    {
        _pressedMouseButtons[(int)button] = state;

        // fire mousebuttonevent
        EventManager& eventMgrInst = EventMgrInstance();

    }

    void InputManager::SetMousePosition(vec2 pos)
    {
        _mousePosition = pos;
    }

    void InputManager::SetMousePosition(float x, float y)
    {
        SetMousePosition(vec2(x, y));
    }

    void InputManager::SetMouseMove(vec2 pos)
    {
        SetMousePosition(pos);

        // fire mouse move event
        EventManager& eventMgrInst = EventMgrInstance();
    }

    void InputManager::SetCursorEnter(vec2 pos, bool entered) 
    { 
        _cursorEntered = entered;
    }

    void InputManager::ReleasePressedMouseButtons()
    {
        for(auto& key : _pressedKeys)
        {
            key = KeyState::KEY_RELEASE;
        }
    }
    
    void InputManager::ReleasePressedKeys()
    {
        for(auto& button : _pressedMouseButtons)
        {
            button = MouseState::MOUSE_RELEASE;
        }
    }

    void InputManager::Release()
    {
        ReleasePressedKeys();
        ReleasePressedMouseButtons();
    }

} // namespace TwinkleGraphics
