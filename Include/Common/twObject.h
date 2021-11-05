
#ifndef TW_OBJECT_H
#define TW_OBJECT_H

#include <memory>

#include "twReference.h"

namespace TwinkleGraphics {
class Object : public Reference<Object> {
public:
  typedef std::shared_ptr<Object> Ptr;
  typedef std::weak_ptr<Object> WeakPtr;

  Object() : Reference<Object>(), _valid(false) {}
  Object(const Object &src) {}
  virtual ~Object() {}
  virtual void SetValid(bool valid) { _valid = valid; }
  virtual bool IsValid() { return _valid; }

protected:
  bool _valid = false;
};

typedef Object::Ptr ObjectPtr;

} // namespace TwinkleGraphics

#endif