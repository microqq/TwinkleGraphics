#ifndef TW_EDITORAPPLICATION_H
#define TW_EDITORAPPLICATION_H

#include "twGLFWMainFrame.h"

namespace TwinkleGraphics
{
    class EditorWindow : public GLFWMainFrame
    {
    public:
        explicit EditorWindow(uint32 width, uint32 height);
        virtual ~EditorWindow();

    private:

    };

} // namespace TwinkleGraphics

#endif