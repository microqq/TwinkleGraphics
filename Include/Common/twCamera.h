

#ifndef TW_CAMERA_H
#define TW_CAMERA_H

#include <glew/glew.h>
//include gl after glew
#include <GL/gl.h>

#include "twCommon.h"
#include "twRenderTexture.h"
#include "twSceneNode.h"

namespace TwinkleGraphics
{
struct Viewport
{
    Rect rect;
    RGBA clearColor;
    float32 clearDepth;
    int32 clearStencil;
    uint32 clearMask;

    inline int32 X() const { return rect.x; }
    inline int32 Y() const { return rect.y; }
    inline int32 Width() const { return rect.z; }
    inline int32 Height() const { return rect.w; }
    inline float32 AspectRatio() const { return (float32)Width() / (float32)Height(); }
    inline void SetRect(const Rect& rect) { this->rect = rect; }

    inline void SetClearMask(uint32 mask) { clearMask = mask; }
    inline void SetClearColor(const RGBA& color) { clearColor = color; }
    inline void SetClearDepth(float32 depth) { clearDepth = depth; }
    inline void SetClearStencil(int32 stencil) { clearStencil = stencil; }
    
    Viewport() {}
    Viewport(const Rect& rect
            , uint32 mask = GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT 
            , const RGBA& color = RGBA(0.0f, 0.0f, 0.0f, 1.0f)
            , float32 depth = 1.0f
            , int32 stencil = 1
    )
        : rect(rect)
        , clearColor(color)
        , clearDepth(depth)
        , clearStencil(stencil)
        , clearMask(mask)
    {}
    Viewport(const Viewport& viewport)
        : rect(viewport.rect)
        , clearColor(viewport.clearColor)
        , clearDepth(viewport.clearDepth)
        , clearStencil(viewport.clearStencil)
        , clearMask(viewport.clearMask)
    {}
    inline Viewport& operator = (const Viewport& viewport)
    {
        if(&viewport == this) return *this;
        this->rect = viewport.rect;
        this->clearColor  = viewport.clearColor;
        this->clearDepth  = viewport.clearDepth;
        this->clearStencil = viewport.clearStencil;
        this->clearMask = viewport.clearMask;
        return *this;
    }
};

enum class FrustumType
{
    PERSPECTIVE,
    ORTHOGRAPHIC
};

class Frustum : public Object
{
public:
    typedef std::shared_ptr<Frustum> Ptr;

    Frustum(float32 fov, float32 aspect, float32 near, float32 far);
    Frustum(float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far);
    virtual ~Frustum()
    {}

protected:
    glm::mat4 _projectionMatrix;
    float32 _fov;
    float32 _aspect;
    float32 _near;
    float32 _far;

    FrustumType _frustumType;
};


class Camera : public Frustum, public ISceneNode
{
public:
    typedef std::shared_ptr<Camera> Ptr;

    Camera(Viewport viewport, float32 fov, float32 near, float32 far);
    Camera(Viewport viewport, float32 near, float32 far);
    virtual ~Camera();

    void LookAt(glm::vec3 center, glm::vec3 up) { _transform->LookAt(center, up); }
    void SetPosition(glm::vec3 position) { _transform->SetPosition(position); }
    void SetOrientation(glm::quat orientation) { _transform->SetOrientation(orientation); }
    void Translate(glm::vec3 trans) { _transform->Translate(trans); }
    void Rotate(glm::vec3 euler) { _transform->Rotate(euler); }
    void Rotate(float32 angle, glm::vec3 axis) { _transform->Rotate(angle, axis); }
    void Rotate(glm::quat quaternion) { _transform->Rotate(quaternion); }
    void ResetTransform() { _transform->Reset(); }

    glm::vec3 GetPosition() { return _transform->GetPosition(); }
    glm::quat GetOrientation() { return _transform->GetWorldToLocalOrientation(); }

    glm::quat GetWorldOrientation() { return _transform->GetLocalToWorldOrientation(); }
    glm::vec3 GetWorldPosition() { return _transform->GetWorldPosition(); }

    void ResizeViewport(float32 scale_x, float32 scale_y)
    {
        _viewport.rect.x *= scale_x;
        _viewport.rect.z *= scale_x;
        _viewport.rect.y *= scale_y;
        _viewport.rect.w *= scale_y;

        _viewportDirty = true;
    }
    void SetViewportRect(Rect rect)
    {
        _viewport.rect = rect;
        _viewportDirty = true;
    }
    void SetViewport(const Viewport& viewport) { _viewport = viewport; _viewportDirty = true; }
    const Viewport& GetViewport() { return _viewport; }

    void ClearRenderContext()
    {
        glViewport(_viewport.X(), _viewport.Y(), _viewport.Width(), _viewport.Height());

        glClear(_viewport.clearMask);
        if ((_viewport.clearMask & GL_COLOR_BUFFER_BIT) != 0)
        {
            const RGBA &color = _viewport.clearColor;
            glClearColor(color.r, color.g, color.b, color.a);
        }
        if ((_viewport.clearMask & GL_DEPTH_BUFFER_BIT) != 0)
        {
            glClearDepth(_viewport.clearDepth);
        }
        if ((_viewport.clearMask & GL_STENCIL_BUFFER_BIT) != 0)
        {
            glClearStencil(_viewport.clearStencil);
        }        
    }

    glm::mat4 GetViewMatrix() { return _transform->GetWorldToLocalMatrix(); }
    const glm::mat4& GetProjectionMatrix() 
    { 
        if(_viewportDirty)
        {
            _aspect = _viewport.AspectRatio();
            if (_frustumType == FrustumType::PERSPECTIVE)
            {
                _projectionMatrix = glm::perspective(_fov, _aspect, _near, _far);
            }
            else
            {
                //compute frustum left\right\bottom...
                //_projection_matrix = glm::ortho(left, right, bottom, top, near, far);
            }
        }
        return _projectionMatrix; 
    }

    void SetRenderToTarget(RenderTexture::Ptr rt) { _rendertarget = rt; }
    void SetCullingMask(CullingMask mask) { _cullingmask |= (int32)mask; }
    void SetCullingMask(int32 mask) { _cullingmask |= mask; }

    int32 GetDepth() { return _sortdepth; }

private:
    Viewport _viewport;
    RenderTexture::Ptr _rendertarget;

    int32 _cullingmask;
    int32 _sortdepth;

    bool _viewportDirty;
};
} // namespace TwinkleGraphics

#endif