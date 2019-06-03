

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

    static Ptr CreateQuadMesh(float32 x, float32 y);
    static Ptr CreateCubeMesh(float32 size);
    static Ptr CreateSphereMesh();

    Mesh();
    virtual ~Mesh();

private:
    
};

class SubMesh : public Object
{
public:
    SubMesh();
    virtual ~SubMesh();

private:

    friend class Mesh;
};

} // namespace TwinkleGraphics


#endif