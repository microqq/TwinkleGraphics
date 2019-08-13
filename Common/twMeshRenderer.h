#ifndef TW_MESHRENDER_H
#define TW_MESHRENDER_H

#include "twCommon.h"
#include "twMesh.h"
#include "twMaterial.h"

namespace TwinkleGraphics
{
class MeshRenderer : public Object
{
public:
    typedef std::shared_ptr<MeshRenderer> Ptr;

    MeshRenderer();
    virtual ~MeshRenderer();

private:
    void GetDrawCommand();
    void GetRenderState();

private:
    Mesh::Ptr _mesh;
};

} // namespace TwinkleGraphics


#endif