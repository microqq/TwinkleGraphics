#ifndef TW_HWOBJECT_H
#define TW_HWOBJECT_H

#include "twCommon.h"

namespace TwinkleGraphics {

class IHWObject : public Reference<IHWObject> {
public:
  using Ptr = std::shared_ptr<IHWObject>;

  IHWObject(int32 type) : Reference<IHWObject>() { _resinstance.type = type; }
  virtual ~IHWObject() {}

  virtual void Create() = 0;
  virtual void Destroy() = 0;
  virtual void Bind() = 0;
  virtual void UnBind() = 0;

  const RenderResourceHandle &GetResource() { return _resinstance; }

protected:
  RenderResourceHandle _resinstance;
};

} // namespace TwinkleGraphics

#endif
