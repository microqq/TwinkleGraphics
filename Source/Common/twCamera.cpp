
#include "twCamera.h"

namespace TwinkleGraphics
{

Frustum::Frustum(float32 fov, float32 aspect, float32 n, float32 f)
    : Object()
    , _fov(fov)
    , _aspect(aspect)
    , _near(n)
    , _far(f)
    , _frustumType(FrustumType::PERSPECTIVE)
{
    _projectionMatrix = glm::perspective(fov, aspect, _near, _far);
}

Frustum::Frustum(float32 left, float32 right, float32 bottom, float32 top, float32 n, float32 f)
    : Object()
    , _aspect((right - left) / (top - bottom))
    , _near(n)
    , _far(n)
    , _frustumType(FrustumType::ORTHOGRAPHIC)
{
    _projectionMatrix = glm::ortho(left, right, bottom, top, _near, _far);
}

Camera::Camera(Viewport viewport, float32 fov, float32 n, float32 f)
    : Frustum(fov, viewport.AspectRatio(), n, f)
    , ISceneNode()
    , _viewport(viewport)
    , _rendertarget(nullptr)
    , _cullingmask((int32)CullingMask::EVERYTHING)
    , _sortdepth(-1)
    , _viewportDirty(true)
{
}

Camera::Camera(Viewport viewport, float32 n, float32 f)
    : Frustum(-viewport.Width() * 0.5f
            , viewport.Width() * 0.5f
            , -viewport.Height() * 0.5f
            , viewport.Height() * 0.5f
            , n, f)
    , ISceneNode()
    , _viewport(viewport)
    , _rendertarget(nullptr)
    , _cullingmask((int32)CullingMask::EVERYTHING)
    , _sortdepth(-1)
    , _viewportDirty(true)
{
}

Camera::~Camera()
{
}

void Camera::Update(float deltaTime)
{

}

void Camera::SetRenderToTarget(RenderTexture::Ptr rt)
{ 
    if(rt != nullptr)
    {
        int32 w = rt->GetWidth();
        int32 h = rt->GetHeight();
        SetViewportRect(Rect(0, 0, w, h));

        _rendertarget = rt; 
    }
}

void Camera::Bind()
{
    if(_rendertarget != nullptr)
    {
        _rendertarget->Bind();
        this->ClearRenderContext();
    }
}

void Camera::UnBind()
{
    if(_rendertarget != nullptr)
    {
        _rendertarget->UnBind();
    }
}

} // namespace TwinkleGraphics
