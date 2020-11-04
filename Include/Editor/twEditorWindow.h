#ifndef TW_EDITORAPPLICATION_H
#define TW_EDITORAPPLICATION_H

#include "twGLFWMainFrame.h"
#include "twGLFWViewWindow.h"
#include "twEditorMenu.h"
#include "twHierarchy.h"
#include "twInspector.h"

namespace TwinkleGraphics
{
    class __TWCOMExport EditorWindow : public GLFWMainFrame
    {
    public:
        explicit EditorWindow(uint32 width, uint32 height);
        virtual ~EditorWindow();

    private:
        EditorMenu* _menu = nullptr;
        Hierarchy* _hierachy = nullptr;
        Inspector* _inspector = nullptr;
        GLFWViewWindow* _sceneView = nullptr;
        GLFWViewWindow* _gameView = nullptr;
    };

} // namespace TwinkleGraphics

#endif