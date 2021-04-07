
#include "twCamera.h"

namespace TwinkleGraphics
{


Camera::Camera(Viewport viewport, float32 fov, float32 n, float32 f, Camera::Type type)
    : SceneNode()
    , _viewport(viewport)
    , _rendertarget(nullptr)
    , _frustum(nullptr)
    , _fov(fov)
    , _aspect(viewport.AspectRatio())
    , _near(n)
    , _far(f)
    , _type(type)
    , _cullingmask((int32)CullingMask::EVERYTHING)
    , _sortdepth(-1)
    , _viewportDirty(true)
{
    SetupProjection();
}

Camera::Camera(Viewport viewport, float32 n, float32 f, Camera::Type type)
    : SceneNode()
    , _viewport(viewport)
    , _rendertarget(nullptr)
    , _near(n)
    , _far(f)
    , _type(type)
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

void Camera::SetRenderToTarget(RenderTexturePtr rt)
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


void Camera::SetupProjection()
{
    if(_type == PERSPECTIVE)
    {
        _projectionMatrix = glm::perspective(_fov, _aspect, _near, _far);
    }
    else
    {

    }
}

} // namespace TwinkleGraphics
