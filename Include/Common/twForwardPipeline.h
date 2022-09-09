
#ifndef TW_FORWARDPIPELINE_H
#define TW_FORWARDPIPELINE_H

#include "twRenderPipeline.h"

namespace TwinkleGraphics {

/**
 * @brief
 *
 */
class ForwardPipeline : public RenderPipeline {
public:
  ForwardPipeline();
  ~ForwardPipeline();
};

using Forward = Singleton<ForwardPipeline>;

} // namespace TwinkleGraphics

#endif