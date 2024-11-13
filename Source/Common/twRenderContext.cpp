
#include "twRenderContext.h"

namespace TwinkleGraphics {
const char *VertexLayoutDefines[11] = {
    "#define VERTCOLOR\n", "#define VERTNOMAL\n", "#define VERTBINORMAL\n",
    "#define VERTUV\n",    "#define VERTUV1\n",   "#define VERTUV2\n",
    "#define VERTUV3\n",   "#define VERTUV4\n",   "#define VERTUV5\n",
    "#define VERTUV6\n",   "#define VERTUV7\n"};

RenderContext::RenderContext() {}

RenderContext::~RenderContext() {}

} // namespace TwinkleGraphics
