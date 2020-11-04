#include "twEditorWindow.h"

namespace TwinkleGraphics
{
    EditorWindow::EditorWindow(uint32 width, uint32 height)
        : GLFWMainFrame(width, height)
    {
        _sceneView = new GLFWViewWindow("SceneView", _data->width * 0.25f, _data->height * 0.25f, this);
        _gameView = new GLFWViewWindow("GameView", _data->width * 0.25f, _data->height * 0.25f, this);

        _menu = new EditorMenu(this);
        _hierachy = new Hierarchy(this);
        _inspector = new Inspector(this);
    }

    EditorWindow::~EditorWindow()
    {
        _sceneView = nullptr;
        _gameView = nullptr;

        _menu = nullptr;
        _hierachy = nullptr;
        _inspector = nullptr;
    }

} // namespace TwinkleGraphics
