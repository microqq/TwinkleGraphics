#ifndef TW_SCENE_NODE_H
#define TW_SCENE_NODE_H

#include "twMeshRenderer.h"
#include "twTransform.h"

namespace TwinkleGraphics
{
class IRenderableObject
{
public:
    IRenderableObject() {}
    ~IRenderableObject() {}

    virtual void SetMeshRenderer(MeshRenderer::Ptr renderer) { _renderer = renderer; }
    virtual MeshRenderer::Ptr GetMeshRenderer() { return _renderer; }

protected:
    MeshRenderer::Ptr _renderer;
};

class ISceneNode
{
public:
    ISceneNode(bool renderable = false)
        : _transform(nullptr)
        , _isrenderable(renderable)
    {
        _transform = std::make_shared<Transform>();
    }
    virtual ~ISceneNode() {}

    Transform::Ptr GetTransform() { return _transform; }
    bool IsRenderable() { return _isrenderable; }

protected:
    Transform::Ptr _transform;
    bool _isrenderable;
};
    
} // namespace TwinkleGraphics
#endif