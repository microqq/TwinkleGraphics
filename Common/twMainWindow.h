

#ifndef TW_MainWINDOW_H
#define TW_MainWINDOW_H

#include <iostream>

#include <glew/glew.h>
//include gl after glew
#include <GL/gl.h>
#include <glfw/glfw3.h>

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
    inline void SetClear_depth(float32 depth) { clear_depth = depth; }
    inline void SetClearStencil(float32 stencil) { clear_stencil = stencil; }
    
    Viewport() {}
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

class MainWindow
{
public:
    MainWindow(int32 width = 800, int32 height = 640);
    virtual ~MainWindow();

    virtual void Run() = 0;
    virtual void Frame();
    void SetViewport(int8 index, const Viewport& viewport);
    void SetViewport(int8 index, const Rect& rect, uint32 mask, const RGBA& color, float32 depth, float32 stencil);

protected:
    virtual void Initialise() = 0;
    virtual void Terminate() = 0;
    virtual void Advance(float64 delta_time = 0.0f);
    virtual void Render() = 0;
    virtual void HandleEvents() = 0;

protected:
    Viewport _viewports[MAX_VIEWPORT_COUNT];

    int32 _width;
    int32 _height;

    int8 _viewport_count;
};

class GLFWMainWindow : public MainWindow
{
public:
    GLFWMainWindow(int32 width = 800, int32 height = 640);
    virtual ~GLFWMainWindow();

    virtual void Run() override;

protected:
    virtual void Initialise() override;
    virtual void Terminate() override;
    virtual void Advance(float64 delta_time = 0.0f) override;
    virtual void Render() override;
    virtual void HandleEvents() override;

private:
    GLFWwindow* _window;
};
} // namespace TwinkleGraphics

#endif