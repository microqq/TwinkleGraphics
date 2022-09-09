
#ifndef TW_OBJECT_H
#define TW_OBJECT_H

#include <memory>

#include "twReference.h"

namespace TwinkleGraphics {
class Object : public Reference<Object> {
public:
  using Ptr = std::shared_ptr<Object>;
  using WeakPtr = std::weak_ptr<Object>;

  Object() : Reference<Object>(), _valid(false) {}
  Object(const Object &src) {}
  virtual ~Object() {}
  virtual void SetValid(bool valid) { _valid = valid; }
  virtual bool IsValid() { return _valid; }

protected:
  bool _valid = false;
};

using ObjectPtr = Object::Ptr;

} // namespace TwinkleGraphics

#endif