
#ifndef TW_VIEW_H
#define TW_VIEW_H


#include <glew/glew.h>
//include gl after glew
#include <GL/gl.h>

#include "twCommon.h"


namespace TwinkleGraphics
{
struct Viewport
{
    Rect rect;
    RGBA clear_color;
    float32 clear_depth;
    int32 clear_stencil;
    uint32 clear_mask;

    inline int32 X() const { return rect.x; }
    inline int32 Y() const { return rect.y; }
    inline int32 Width() const { return rect.z; }
    inline int32 Height() const { return rect.w; }
    inline float32 AspectRatio() const { return (float32)Width() / (float32)Height(); }
    inline void SetRect(const Rect& rect) { this->rect = rect; }

    inline void SetClearMask(uint32 mask) { clear_mask = mask; }
    inline void SetClearColor(const RGBA& color) { clear_color = color; }
    inline void SetClearDepth(float32 depth) { clear_depth = depth; }
    inline void SetClearStencil(int32 stencil) { clear_stencil = stencil; }
    
    Viewport() {}
    Viewport(const Rect& rect
            , uint32 mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT 
            , const RGBA& color = RGBA(0.0f, 0.0f, 0.0f, 1.0f)
            , float32 depth = 1.0f
            , int32 stencil = 1
    )
        : rect(rect)
        , clear_color(color)
        , clear_depth(depth)
        , clear_stencil(stencil)
        , clear_mask(mask)
    {}
    Viewport(const Viewport& viewport)
        : rect(viewport.rect)
        , clear_color(viewport.clear_color)
        , clear_depth(viewport.clear_depth)
        , clear_stencil(viewport.clear_stencil)
        , clear_mask(viewport.clear_mask)
    {}
    inline Viewport& operator = (const Viewport& viewport)
    {
        if(&viewport == this) return *this;
        this->rect = viewport.rect;
        this->clear_color  = viewport.clear_color;
        this->clear_depth  = viewport.clear_depth;
        this->clear_stencil = viewport.clear_stencil;
        this->clear_mask = viewport.clear_mask;
        return *this;
    }
};


class View
{
public:
    View(Viewport& viewport);
    ~View();

    inline Viewport& GetViewport() 
    { return _viewport; }

    void operator = (const Viewport& viewport);

    virtual void Run();
    virtual void Frame();
    virtual void Advance(float64 delta_time);
    virtual void Render();
    virtual void HandleEvents();

    inline void SetRenderRoutine(RENDER_ROUTINE routine)
    {
        _render_routine_func = routine;
    }

protected:
    virtual void RenderImplement() {}

protected:
    Viewport _viewport;
    RENDER_ROUTINE _render_routine_func;

    bool _done;
};

} // namespace TwinkleGraphics


#endif