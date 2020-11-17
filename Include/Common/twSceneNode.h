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
    virtual ~IRenderableObject() {}

    virtual void SetMeshRenderer(MeshRenderer::Ptr renderer) { _renderer = renderer; }
    virtual MeshRenderer::Ptr GetMeshRenderer() { return _renderer; }

protected:
    MeshRenderer::Ptr _renderer;
};

enum class SceneLayerType
{
    NONE = 0,
    DEFAULT = 1,        //solid objects
    TRANSPARENTS = 2,
    UI = 4,
    POSTPROCESSING = 8,
    EVERYTHING = DEFAULT | TRANSPARENTS | UI | POSTPROCESSING // 15
};
typedef SceneLayerType CullingMask;

class ISceneNode
{
public:
    typedef std::shared_ptr<ISceneNode> Ptr;

    ISceneNode(bool renderable = false)
        : _transform(nullptr)
        , _isrenderable(renderable)
        , _enabled(true)
    {
        _transform = std::make_shared<Transform>();
    }
    virtual ~ISceneNode() {}

    virtual void Updata(float deltaTime = 0.0f) {}

    Transform::Ptr GetTransform() { return _transform; }
    bool IsRenderable() { return _isrenderable; }

    void SetEnabled(bool enabled) { _enabled = enabled; }
    bool Enabled() { return _enabled; }

protected:
    Transform::Ptr _transform;
    bool _isrenderable = false;
    bool _enabled = true;
};
    
} // namespace TwinkleGraphics
#endif