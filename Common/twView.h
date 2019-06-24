
#ifndef TW_VIEW_H
#define TW_VIEW_H


// #include <glew/glew.h>
// //include gl after glew
// #include <GL/gl.h>

// #include "twCommon.h"

#include "twCamera.h"


namespace TwinkleGraphics
{
class View
{
public:
    View(Viewport& viewport);
    ~View();

    const Viewport& GetViewport() 
    { return _viewport; }

    void Resize(float32 scale_x, float32 scale_y)
    {
        _viewport.rect.x *= scale_x;
        _viewport.rect.z *= scale_x;
        _viewport.rect.y *= scale_y;
        _viewport.rect.w *= scale_y;
    }

    void Reset(Rect rect)
    {
        _viewport.rect = rect;
    }

    View& operator = (const Viewport& viewport);
    View& operator = (const View& view);

    void Run();
    void OnViewGUI() { this->OnGUI(); }

protected:
    virtual void Initialize() {}
    virtual void Advance(float64 delta_time);
    virtual void HandleEvents();
    virtual void RenderImpl() {}
    virtual void Destroy() {}
    virtual void OnGUI() {}

private:
    void Render();

protected:
    Viewport _viewport;

    bool _done;
};

} // namespace TwinkleGraphics


#endif