#ifndef TW_APPLICATION_H
#define TW_APPLICATION_H

#include "twCommon.h"
#include "twMainFrame.h"

namespace TwinkleGraphics
{
    class Application : public INonCopyable, public IUpdatable, public IDestroyable
    {
    public:
        ~Application();
        virtual void Update(float deltaTime = 0.0f) override;
        virtual void Destroy() override;

        virtual void Init();
        virtual void Run();
        virtual void Quit();

        MainFrame* GetMainFrame() { return _mainframe; }

        void MouseInputCallback(int32 button, int32 action, int32 mods) {}
        void CursorPosCallback(float64 xpos, float64 ypos) {}
        void CursorEnterPosCallback(int32 entered) {}
        void ScrollCallback(float64 dx, float64 dy) {}
        void WindowSizeCallback(int32 w, int32 h) {}
        void KeyInputCallBack(int32 key, int32 scannode, int32 action, int32 mods) {}

    protected:
        explicit Application();

    protected:
        MainFrame* _mainframe = nullptr;

        friend class Singleton<Application>;
        friend class MainFrame;
    };

    __TWCOMExport Application &ApplicationInstance();
} // namespace TwinkleGraphics

#endif