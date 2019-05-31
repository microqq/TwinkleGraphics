

#ifndef TW_MESH_H
#define TW_MESH_H

#include "twCommon.h"

namespace TwinkleGraphics
{
struct Vertex
{

};

class Mesh : public Object
{
public:
    typedef std::shared_ptr<Mesh> Ptr;
    typedef std::weak_ptr<Mesh> WeakPtr;

    static Ptr CreateQuadMesh(float32 x, float y);
    static Ptr CreateCubeMesh(float32 size);
    static Ptr CreateShpereMesh();

    Mesh();
    virtual ~Mesh();
};

} // namespace TwinkleGraphics


#endif