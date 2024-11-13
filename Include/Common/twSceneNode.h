#ifndef TW_SCENE_NODE_H
#define TW_SCENE_NODE_H

#include <vector>

#include "twMeshRenderer.h"
#include "twTransform.h"

namespace TwinkleGraphics {
class Scene;
class SceneManager;

class RenderableObject {
public:
  RenderableObject() {}
  virtual ~RenderableObject() {}

  virtual void SetMeshRenderer(MeshRendererPtr renderer) {
    _renderer = renderer;
  }
  virtual MeshRendererPtr GetMeshRenderer() { return _renderer; }

protected:
  MeshRendererPtr _renderer;
};

enum class SceneLayerType {
  NONE = 0,
  DEFAULT = 1, // solid objects
  TRANSPARENTS = 2,
  UI = 4,
  POSTPROCESSING = 8,
  EVERYTHING = DEFAULT | TRANSPARENTS | UI | POSTPROCESSING // 15
};
using CullingMask = SceneLayerType;

class __TWAPI SceneNode : public Object, public IUpdatable {
public:
  using Ptr = std::shared_ptr<SceneNode>;

  SceneNode(bool renderable = false)
      : Object(), IUpdatable(), _children(), _parent(nullptr),
        _transform(nullptr), _renderable(renderable), _enabled(true) {
    _transform = std::make_shared<Transform>();
    _transform->SetOwner(this);
  }
  virtual ~SceneNode();

  virtual void Update(float deltaTime = 0.0f) override;

  void AddChild(SceneNode::Ptr node);
  void RemoveChild(SceneNode::Ptr node);
  void AttachToParent(SceneNode::Ptr parent);
  void DetachFromParent();
  SceneNode::Ptr GetParent() { return _parent; }
  TransformPtr GetParentTransform() {
    return _parent != nullptr ? _parent->_transform : nullptr;
  }
  int32 GetChildrenCount() { return _children.size(); }

  TransformPtr GetTransform() { return _transform; }
  void SetRenderable(bool renderable) { _renderable = renderable; }
  bool Renderable() { return _renderable; }

  void SetEnabled(bool enabled) { _enabled = enabled; }
  bool Enabled() { return _enabled; }

protected:
  using Iterator = std::vector<SceneNode::Ptr>::iterator;

  bool HasChild(SceneNode::Ptr node);
  Iterator FindChild(SceneNode::Ptr node);

protected:
  std::vector<SceneNode::Ptr> _children;
  SceneNode::Ptr _parent = nullptr;
  TransformPtr _transform;
  bool _renderable = false;
  bool _enabled = true;

  friend class Scene;
  friend class SceneManager;
};

using SceneNodePtr = SceneNode::Ptr;

} // namespace TwinkleGraphics
#endif