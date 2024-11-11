#ifndef TW_CAMERACONTROL_H
#define TW_CAMERACONTROL_H

#include <memory>

#include "twCamera.h"

namespace TwinkleGraphics {
class __TWAPI CameraControl : public Object {
public:
  using Ptr = std::shared_ptr<CameraControl>;

  CameraControl(CameraPtr camera) : _camera(camera) {}
  virtual ~CameraControl() {}

  void SetCamera(CameraPtr camera) { _camera = camera; }
  CameraPtr GetCamera() { return _camera; }
  virtual void UpdateCamera() {}

  virtual void HandleMouseMove(glm::dvec2 move) {}
  virtual void HandleMouseLeftButtonDrag(glm::dvec2 p1, glm::dvec2 p2) {}
  virtual void HandleMouseRightButtonDrag(glm::dvec2 p1, glm::dvec2 p2) {}
  virtual void HandleMouseScroll(glm::dvec2 scroll) {}
  virtual void HandleKeyPress(int32 key, int32 scannode, int32 action,
                              int32 mods) {}
  virtual void HandleKeyRelease(int32 key, int32 scannode, int32 action,
                                int32 mods) {}

protected:
  CameraPtr _camera;
};

using CameraControlPtr = CameraControl::Ptr;

} // namespace TwinkleGraphics

#endif