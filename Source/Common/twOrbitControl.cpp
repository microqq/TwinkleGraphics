

#include <iostream>

#include "twOrbitControl.h"

namespace TwinkleGraphics {
OrbitControl::OrbitControl(CameraPtr camera)
    : CameraControl(camera), _center(), _maxDistance(500.0f),
      _minDistance(5.0f), _distance(25.0f), _zoomSpeed(1.0f), _radius(10.0f),
      _rotateX(0.0f), _rotateY(0.0f), _dirty(true) {
  Initialize();
}

OrbitControl::~OrbitControl() {}

void OrbitControl::Initialize() {
  if (_target == nullptr) {
    _target = std::make_shared<SceneNode>();
  }

  _target->AddChild(_camera);
  _camera->ResetTransform();
  _camera->Translate(glm::vec3(0.0f, 0.0f, _distance));
}

/// Updates the camera's view and projection matrices if the OrbitControl object
/// has been marked as dirty. This ensures the camera's matrices are up-to-date
/// with any changes made to the OrbitControl object.
void OrbitControl::UpdateCamera() {
  // translate\rotate...

  if (_dirty) {
    _camera->GetViewMatrix();
    _camera->GetProjectionMatrix();

    _dirty = false;
  }
}

/// Zooms the camera in or out based on the provided factor.
/// The camera's distance from the target is adjusted based on the y-component
/// of the factor. The distance is clamped between the minimum and maximum
/// distance values set in the OrbitControl object.
/// After adjusting the distance, the camera is translated along the z-axis
/// to update its position.
/// The OrbitControl object is marked as dirty after the zoom operation,
/// so the camera's view and projection matrices will be updated on the
/// next call to UpdateCamera().
void OrbitControl::Zoom(glm::vec2 factor) {
  float32 dy = factor.y;

  float32 distance = _distance - dy;
  float32 offset = _distance;
  if (distance < _minDistance)
    _distance = _minDistance;
  else if (distance > _maxDistance)
    _distance = _maxDistance;
  else {
    _distance = distance;
  }

  offset = _distance - offset;

  // glm::mat3 rotateMat =
  // glm::mat3_cast(_camera->GetTransform()->GetLocalToWorldOrientation());
  // _camera->Translate(rotateMat * glm::vec3(0.0f, 0.0f, offset));
  _camera->Translate(glm::vec3(0.0f, 0.0f, offset));

  _dirty = true;
}


/**
 * @brief Pans the camera by the difference between the two input positions.
 * The input positions are in screen space coordinates, and are normalized to the
 * range [0, 1] based on the camera's viewport size. The resulting pan vector is
 * then applied to the camera's translation, effectively moving the camera in the
 * opposite direction.
 * 
 * @param p1 The first input position in screen space.
 * @param p2 The second input position in screen space.
 */
void OrbitControl::Pan(glm::vec2 p1, glm::vec2 p2) {
  glm::vec2 screenSize = glm::vec2(_camera->GetViewport().Width(),
                                    _camera->GetViewport().Height());

  glm::vec2 sphericalP1 = p1 / screenSize;
  glm::vec2 sphericalP2 = p2 / screenSize;

  sphericalP1 = (sphericalP1 - glm::vec2(0.5f, 0.5f)) * 2.0f * _radius;
  sphericalP2 = (sphericalP2 - glm::vec2(0.5f, 0.5f)) * 2.0f * _radius;
  glm::vec2 p = sphericalP2 - sphericalP1;
  glm::vec3 d = glm::vec3(-p.x, p.y, 0.0f);

  // glm::mat3 rotateMat =
  // glm::mat3_cast(_camera->GetTransform()->GetLocalToWorldOrientation()); d =
  // rotateMat * d;

  _camera->Translate(d);

  _dirty = true;
}


/**
 * @brief Applies a trackball-style rotation to the camera's orientation.
 * The trackball rotation is calculated based on the difference between two input
 * positions in screen space. The resulting rotation is then applied to the
 * target object's orientation.
 * 
 * @param p1 The first input position in screen space.
 * @param p2 The second input position in screen space.
 */
void OrbitControl::Trackball(glm::vec2 p1, glm::vec2 p2) {
  glm::vec2 screenSize = glm::vec2(_camera->GetViewport().Width(),
                                    _camera->GetViewport().Height());

  glm::vec2 sphericalP1 = p1 / screenSize;
  glm::vec2 sphericalP2 = p2 / screenSize;

  sphericalP1 = (sphericalP1 - glm::vec2(0.5f, 0.5f)) * 2.0f * _radius;
  sphericalP2 = (sphericalP2 - glm::vec2(0.5f, 0.5f)) * 2.0f * _radius;
  glm::vec2 p = sphericalP2 - sphericalP1;

  glm::vec3 v0(0.0f, 0.0f, 1.0f);
  glm::vec3 v1 = glm::normalize(glm::vec3(p.x, 0.0f, _radius));
  glm::vec3 v2 = glm::normalize(glm::vec3(0.0f, p.y, _radius));

  float32 factor = 1.0f;

  float32 thetaX = glm::acos(glm::dot(v0, v1));
  if (::fabs(thetaX) > glm::epsilon<float32>()) {
    factor = p.x > 0.0f ? 1.0f : -1.0f;
    _rotateX += thetaX * factor;
  }

  float32 thetaY = glm::acos(glm::dot(v0, v2));
  if (::fabs(thetaY) > glm::epsilon<float32>()) {
    factor = p.y > 0.0f ? -1.0f : 1.0f;
    _rotateY += thetaY * factor;
  }

  TransformPtr targetTrans = _target->GetTransform();

  targetTrans->SetOrientation(glm::identity<glm::quat>());
  targetTrans->Rotate(glm::vec3(+_rotateY, -_rotateX, 0.0f));

  _dirty = true;
}

} // namespace TwinkleGraphics
