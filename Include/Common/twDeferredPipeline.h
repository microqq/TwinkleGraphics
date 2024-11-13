

#ifndef TW_DEFERREDPIPELINE_H
#define TW_DEFERREDPIPELINE_H

#include "twRenderPipeline.h"

namespace TwinkleGraphics {

/**
 * @brief
 *
 */
class DeferredPipeline : public RenderPipeline {
public:
  DeferredPipeline();
  ~DeferredPipeline();
};

using Deferred = Singleton<DeferredPipeline>;

} // namespace TwinkleGraphics

#endif