
#ifndef TW_REFERENCE_H
#define TW_REFERENCE_H

#include <memory>

namespace TwinkleGraphics {
template <class T> class Reference : public std::enable_shared_from_this<T> {
public:
  Reference() {}
  virtual ~Reference() {}
};
} // namespace TwinkleGraphics

#endif