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
        , _pressedKeys{false}
        , _pressedMouseButtons{false}
    {
    }

    InputManager::~InputManager()
    {
    }

    void InputManager::Update()
    {
    }

    bool InputManager::GetPressedKey(KeyCode key) 
    {
        return _pressedKeys[(int)key];
    }

    bool InputManager::GetPressedMouse(MouseButton button) 
    {
        return _pressedMouseButtons[(int)button];
    }

    void InputManager::SetPressedKey(KeyCode key, bool pressed) 
    {
        _pressedKeys[(int)key] = pressed;
    }

    void InputManager::SetPressedMouse(MouseButton button, bool pressed) 
    {
        _pressedMouseButtons[(int)button] = pressed;
    }

    void InputManager::SetMousePosition(vec2 pos)
    {
        _mousePosition = pos;
    }

    void InputManager::SetMousePosition(float x, float y)
    {
        _mousePosition = vec2(x, y);
    }


    void InputManager::ReleasePressedMouseButtons()
    {
        for(auto& key : _pressedKeys)
        {
            key = false;
        }
    }
    
    void InputManager::ReleasePressedKeys()
    {
        for(auto& button : _pressedMouseButtons)
        {
            button = false;
        }
    }

    void InputManager::Release()
    {
        ReleasePressedKeys();
        ReleasePressedMouseButtons();
    }

} // namespace TwinkleGraphics
