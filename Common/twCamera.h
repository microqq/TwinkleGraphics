

#ifndef TW_CAMERA_H
#define TW_CAMERA_H

#include <glew/glew.h>
//include gl after glew
#include <GL/gl.h>

#include "twCommon.h"
#include "twTransform.h"

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

enum class FrustumType
{
    PERSPECTIVE,
    ORTHOGRAPHIC
};

class Frustum
{
public:
    Frustum(float32 fov, float32 aspect, float32 near, float32 far, FrustumType type = FrustumType::PERSPECTIVE);
    Frustum(float32 left, float32 right, float32 bottom, float32 top, float32 near, float32 far, FrustumType type = FrustumType::ORTHOGRAPHIC);
    ~Frustum()
    {}

protected:
    glm::mat4 _projection_matrix;
    float32 _fov;
    float32 _aspect;
    float32 _near;
    float32 _far;

    FrustumType _frustum_type;
};

class Camera final : public Object, public Frustum
{
public:
    typedef std::shared_ptr<Camera> Ptr;

    Camera(Viewport viewport, float32 fov, float32 near, float32 far, FrustumType type = FrustumType::PERSPECTIVE);
    ~Camera();

    void LookAt(glm::vec3 center, glm::vec3 up) { _transform->LookAt(center, up); }
    void SetPosition(glm::vec3 position) { _transform->SetPosition(position); }
    void SetOrientation(glm::quat orientation) { _transform->SetOrientation(orientation); }
    void Translate(glm::vec3 trans) { _transform->Translate(trans); }
    void Rotate(glm::vec3 euler) { _transform->Rotate(euler); }
    void Rotate(float32 angle, glm::vec3 axis) { _transform->Rotate(angle, axis); }
    void Rotate(glm::quat quaternion) { _transform->Rotate(quaternion); }
    void ResetTransform() { _transform->Reset(); }

    Transform::Ptr GetTransform() { return _transform; }
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

        _viewport_dirty = true;
    }
    void SetViewportRect(Rect rect)
    {
        _viewport.rect = rect;
        _viewport_dirty = true;
    }
    void SetViewport(const Viewport& viewport) { _viewport = viewport; _viewport_dirty = true; }
    const Viewport& GetViewport() { return _viewport; }

    glm::mat4 GetViewMatrix() { return _transform->GetWorldToLocalMatrix(); }
    const glm::mat4& GetProjectionMatrix() 
    { 
        if(_viewport_dirty)
        {
            _aspect = _viewport.AspectRatio();
            if (_frustum_type == FrustumType::PERSPECTIVE)
            {
                _projection_matrix = glm::perspective(_fov, _aspect, _near, _far);
            }
            else
            {
                //compute frustum left\right\bottom...
                //_projection_matrix = glm::ortho(left, right, bottom, top, near, far);
            }            
        }
        return _projection_matrix; 
    }

private:
    Viewport _viewport;
    Transform::Ptr _transform;

    bool _viewport_dirty;
};
} // namespace TwinkleGraphics

#endif