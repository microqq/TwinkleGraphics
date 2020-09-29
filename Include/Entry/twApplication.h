#ifndef TW_APPLICATION_H
#define TW_APPLICATION_H

#include "twCommon.h"

namespace TwinkleGraphics
{
    class Application final : public INonCopyable, public IUpdatable, public IDestroyable
    {
    public:
        ~Application();
        virtual void Update() override;
        virtual void Destroy() override;

        void Init();
        void Run();
        void Quit();

        void MouseInputCallback(int32 button, int32 action, int32 mods) {}
        void CursorPosCallback(float64 xpos, float64 ypos) {}
        void CursorEnterPosCallback(int32 entered) {}
        void ScrollCallback(float64 dx, float64 dy) {}
        void WindowSizeCallback(int32 w, int32 h) {}
        void KeyInputCallBack(int32 key, int32 scannode, int32 action, int32 mods) {}

    private:
        explicit Application();

        friend class Singleton<Application>;
    };

    __TWCOMExport Application &ApplicationInstance();
} // namespace TwinkleGraphics

#endif