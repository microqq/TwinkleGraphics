

#ifndef TW_TRANSFORM_H
#define TW_TRANSFORM_H

#include <memory>
#include <vector>

#include "twCommon.h"

namespace TwinkleGraphics {
enum class Space { SELF, PARENT, WORLD };

class SceneNode;

class __TWAPI Transform final : public Object {
public:
  using Ptr = std::shared_ptr<Transform>;

  Transform()
      : Object(), _owner(nullptr), _localMatrix(glm::identity<glm::mat4>()),
        _worldMatrix(glm::identity<glm::mat4>()),
        _orientation(glm::identity<glm::quat>()),
        _position(glm::vec3(0.0f, 0.0f, 0.0f)),
        _scale(glm::vec3(1.0f, 1.0f, 1.0f)), _localDirty(true) {}

  ~Transform() {}

  void Translate(glm::vec3 position);
  void Translate(float trans, glm::vec3 axis);
  void Rotate(float32 angle, glm::vec3 axis);
  void Rotate(glm::quat quaternion);
  void Rotate(glm::vec3 euler);
  void Scale(glm::vec3 scale);
  void Scale(float scale, glm::vec3 axis);
  void LookAt(glm::vec3 center, glm::vec3 up);
  void Reset();

  inline glm::vec3 GetPosition() { return _position; }
  inline glm::quat GetOrientation() { return _orientation; }
  inline glm::vec3 GetEuler() { return glm::eulerAngles(_orientation); }
  inline glm::vec3 GetScale() { return _scale; }

  glm::vec3 GetWorldPosition();
  glm::vec3 GetWorldScale();
  glm::quat GetWorldToLocalOrientation();
  glm::quat GetLocalToWorldOrientation();

  inline void SetPosition(glm::vec3 position) {
    _position = position;
    _localDirty = true;
  }
  inline void SetOrientation(glm::quat orientation) {
    _orientation = orientation;
    _localDirty = true;
  }
  inline void SetScale(glm::vec3 scale) {
    _scale = scale;
    _localDirty = true;
  }

  glm::mat4 GetWorldToLocalMatrix();
  const glm::mat4 GetLocalMatrix() { return _localMatrix; }
  const glm::mat4 &GetLocalToWorldMatrix();

  void SetOwner(SceneNode *owner) { _owner = owner; }
  SceneNode *GetOwner() { return _owner; }

  inline void MarkDirty() { _localDirty = true; }
  inline bool DirtyFlag() { return _localDirty; }

  void Update();

private:
  bool WorldDirty();
  void ComputeLocalMatrix();
  void ComputeWorldMatrix();

private:
  SceneNode *_owner = nullptr;

  // transform relative to parent transform
  glm::mat4 _localMatrix;
  // world transform
  glm::mat4 _worldMatrix;

  // orientation relative to parent
  glm::quat _orientation;
  glm::vec3 _position;
  glm::vec3 _scale;

  bool _localDirty;
};

using TransformPtr = Transform::Ptr;
} // namespace TwinkleGraphics

#endif
