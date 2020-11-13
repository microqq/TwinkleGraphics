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