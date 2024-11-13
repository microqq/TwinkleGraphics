

#include "twTransform.h"
#include "twSceneNode.h"

namespace TwinkleGraphics {
void Transform::Translate(glm::vec3 position) {
  _position += position;
  _localDirty = true;
}

void Transform::Translate(float trans, glm::vec3 axis) {
  _position += glm::normalize(axis) * trans;
  _localDirty = true;
}

void Transform::Rotate(float32 angle, glm::vec3 axis) {
  glm::quat newQuat(::cosf(angle * 0.5f),
                     ::sinf(angle * 0.5f) * glm::normalize(axis));
  _orientation = glm::normalize(newQuat * _orientation);
  _localDirty = true;
}

void Transform::Rotate(glm::quat quaternion) {
  _orientation = glm::normalize(quaternion * _orientation);
  _localDirty = true;
}

/**
 * @brief
 * euler:(x, y, z)-(pitch, yaw, roll)
 * @param euler
 */
void Transform::Rotate(glm::vec3 euler) {
  glm::quat eulerQuat = glm::quat(euler);
  _orientation = glm::normalize(glm::inverse(eulerQuat) * _orientation);
  _localDirty = true;
}

void Transform::Scale(glm::vec3 scale) {
  _scale *= scale;
  _localDirty = true;
}

void Transform::Scale(float scale, glm::vec3 axis) { _localDirty = true; }

void Transform::LookAt(glm::vec3 center, glm::vec3 up) {
  glm::mat4 matLookat = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), center, up);
  _orientation = glm::quat_cast(matLookat);

  _localDirty = true;
}

void Transform::Reset() {
  _position = glm::vec3();
  _orientation = glm::identity<glm::quat>();
  _scale = glm::vec3(1.0f, 1.0f, 1.0f);

  _localDirty = true;
}

glm::vec3 Transform::GetWorldPosition() {
  TransformPtr ownerParentTrans = _owner->GetParentTransform();
  if (ownerParentTrans == nullptr) {
    return GetPosition();
  } else {
    glm::vec4 worldPosition =
        ownerParentTrans->GetLocalToWorldMatrix() * glm::vec4(_position, 1.0f);
    return glm::vec3(worldPosition);
  }
}

glm::quat Transform::GetWorldToLocalOrientation() {
  TransformPtr ownerParentTrans = _owner->GetParentTransform();
  if (ownerParentTrans == nullptr) {
    return _orientation;
  } else {
    return ownerParentTrans->GetWorldToLocalOrientation() * _orientation;
  }
}

glm::quat Transform::GetLocalToWorldOrientation() {
  TransformPtr ownerParentTrans = _owner->GetParentTransform();
  if (ownerParentTrans == nullptr) {
    return glm::inverse(_orientation);
  } else {
    return glm::inverse(_orientation) *
           ownerParentTrans->GetLocalToWorldOrientation();
  }
}

glm::vec3 Transform::GetWorldScale() {
  TransformPtr ownerParentTrans = _owner->GetParentTransform();
  if (ownerParentTrans == nullptr) {
    return GetScale();
  } else {
    return ownerParentTrans->GetWorldScale() * _scale;
  }
}

glm::mat4 Transform::GetWorldToLocalMatrix() {
  ComputeLocalMatrix();

  TransformPtr ownerParentTrans = _owner->GetParentTransform();
  if (ownerParentTrans != nullptr) {
    return _localMatrix * ownerParentTrans->GetWorldToLocalMatrix();
  } else {
    return _localMatrix;
  }
}

const glm::mat4 &Transform::GetLocalToWorldMatrix() {
  ComputeWorldMatrix();
  return _worldMatrix;
}

void Transform::Update() {
  ComputeLocalMatrix();
  ComputeWorldMatrix();
}

bool Transform::WorldDirty() {
  TransformPtr ownerParentTrans = _owner->GetParentTransform();
  if (ownerParentTrans == nullptr)
    return _localDirty;
  else {
    return ownerParentTrans->WorldDirty() || _localDirty;
  }
}

void Transform::ComputeLocalMatrix() {
  if (_localDirty) {
    glm::mat4 translateMatrix = glm::identity<glm::mat4>();
    translateMatrix = glm::translate(translateMatrix, -_position);

    glm::mat4 scaleMatrix = glm::identity<glm::mat4>();
    scaleMatrix = glm::scale(scaleMatrix, 1.0f / _scale);

    glm::mat4 rotateMatrix = glm::mat4_cast(_orientation);
    _localMatrix = scaleMatrix * rotateMatrix * translateMatrix;

    _localDirty = false;
  }
}

void Transform::ComputeWorldMatrix() {
  if (WorldDirty()) {
    ComputeLocalMatrix();

    TransformPtr ownerParentTrans = _owner->GetParentTransform();
    if (ownerParentTrans != nullptr) {
      _worldMatrix = ownerParentTrans->GetLocalToWorldMatrix() *
                     glm::inverse(_localMatrix);
    } else {
      _worldMatrix = glm::inverse(_localMatrix);
    }
  }
}
} // namespace TwinkleGraphics
