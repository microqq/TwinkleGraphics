

#ifndef TW_DEFERREDPIPELINE_H
#define TW_DEFERREDPIPELINE_H

#include "twRenderPipeline.h"

namespace TwinkleGraphics
{

class DeferredPipeline : public RenderPipeline
{
public:
    DeferredPipeline();
    ~DeferredPipeline();
};

typedef Singleton<DeferredPipeline> Forward;

} // namespace TwinkleGraphics



#endif