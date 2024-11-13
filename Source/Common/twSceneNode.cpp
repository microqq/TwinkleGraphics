#include "twSceneNode.h"

namespace TwinkleGraphics {
SceneNode::~SceneNode() { _children.clear(); }

void SceneNode::Update(float deltaTime) { _transform->Update(); }

void SceneNode::AddChild(SceneNodePtr node) {
  if (HasChild(node)) {
    return;
  }

  SceneNodePtr oldParent = node->_parent;
  if (oldParent != nullptr) {
    oldParent->RemoveChild(node);
  }
  _children.emplace_back(node);

  SceneNodePtr thisParent =
      std::dynamic_pointer_cast<SceneNode>(shared_from_this());
  node->_parent = thisParent;
  node->_transform->MarkDirty();
}

void SceneNode::RemoveChild(SceneNodePtr node) {
  Iterator iter = FindChild(node);
  if (iter == _children.end()) {
    return;
  }

  _children.erase(iter);
  node->_parent = nullptr;
  node->_transform->MarkDirty();
}

void SceneNode::AttachToParent(SceneNodePtr parent) {
  if (parent == nullptr || _parent == parent) {
    return;
  }

  SceneNodePtr thisNode =
      std::dynamic_pointer_cast<SceneNode>(shared_from_this());
  parent->AddChild(thisNode);
}

void SceneNode::DetachFromParent() {
  if (_parent == nullptr) {
    return;
  }

  SceneNodePtr thisNode =
      std::dynamic_pointer_cast<SceneNode>(shared_from_this());
  _parent->RemoveChild(thisNode);
}

bool SceneNode::HasChild(SceneNodePtr node) {
  return FindChild(node) != _children.end();
}

SceneNode::Iterator SceneNode::FindChild(SceneNodePtr node) {
  if (node == nullptr) {
    return _children.end();
  }

  Iterator iter =
      std::find_if(_children.begin(), _children.end(),
                   [node](SceneNodePtr child) { return child == node; });

  return iter != _children.end() ? iter : _children.end();
}

} // namespace TwinkleGraphics
