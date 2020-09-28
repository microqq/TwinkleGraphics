#include "twInput.h"

namespace TwinkleGraphics
{
    InputManager& InputMgrInstance() { return Singleton<InputManager>::Instance(); }

    InputManager::InputManager()
        : IUpdatable()
        , INonCopyable()
    {}

    InputManager::~InputManager()
    {}    

    void InputManager::Update()
    {}
} // namespace TwinkleGraphics
