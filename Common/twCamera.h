

#ifndef TW_CAMERA_H
#define TW_CAMERA_H

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

class Frustum
{
public:
    enum class FrustumType
    {
        PERSPECTIVE,
        ORTHOGRAPHIC
    };

    Frustum(float32 fov, float32 aspect, float32 near, float32 far, FrustumType type = FrustumType::PERSPECTIVE);
    Frustum(float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far, FrustumType type = FrustumType::ORTHOGRAPHIC);
    ~Frustum()
    {}

protected:
    glm::mat4 _projection_matrix;
};

class Camera final : public Object, public Frustum
{
public:
    Camera(Viewport viewport, float32 fov, float32 near, float32 far, FrustumType type = FrustumType::PERSPECTIVE);
    ~Camera();

    void SetViewport(const Viewport& viewport) { _viewport = viewport; }
    const Viewport& GetViewport() { return _viewport; }

    const glm::mat4& GetViewMatrix() { return _view_matrix; }
    const glm::mat4& GetProjectionMatrix() { return _projection_matrix; }

private:
    glm::mat4 _view_matrix;
    Viewport _viewport;


};
} // namespace TwinkleGraphics

#endif