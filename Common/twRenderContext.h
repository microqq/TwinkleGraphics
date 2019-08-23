
#ifndef TW_RENDERCONTEXT_H
#define TW_RENDERCONTEXT_H

#include <glew/glew.h>
#include "twCommon.h"

namespace TwinkleGraphics
{

struct StateAttribute
{

};

/*------------------------------Drew Command--------------------------*/

struct DrawCommand
{

};

enum class PrimitiveType
{
    POINTS = GL_POINTS,

    LINES =  GL_LINES,
    LINE_STRIP = GL_LINE_STRIP,
    LINES_ADJACENCY = GL_LINES_ADJACENCY,
    LINE_STRIP_ADJACENCY = GL_LINE_STRIP_ADJACENCY,

    TRIANGLES = GL_TRIANGLES,
    TRIANGLE_STRIP = GL_TRIANGLE_STRIP,
    TRIANGLES_ADJACENCY = GL_TRIANGLES_ADJACENCY,
    TRIANGLE_STRIP_ADJACENCY = GL_TRIANGLE_STRIP_ADJACENCY,

    QUADS = GL_QUADS,
    QUAD_STRIP = GL_QUAD_STRIP
};


/*------------------------------Render State--------------------------*/

struct RenderState
{

};


struct RenderStateAttribute : public StateAttribute
{

};

/*------------------------------Raster State--------------------------*/

enum class CullMode
{
    NONE = GL_NONE,
};
enum class PolygonMode
{
    
};
struct RasterState
{

};

struct RasterStateAttribute : public StateAttribute
{
    
};



/*------------------------------Per Fragment Operation--------------------------*/

struct PerFragmentOperation
{

};

enum class DepthFunc
{};

enum class BlendFunc
{};


/*------------------------------Vertex Attribute--------------------------*/

struct VertexLayout
{

};

struct VertexLayoutAttrib
{

};



/*------------------------------Uniform--------------------------*/

enum UniformType
{
    UNI_FLOAT,
    UNI_DOUBLE,
    UNI_UINT,
    UNI_INT,
    UNI_BOOL,
    UNI_FVEC1,
    UNI_FVEC2,
    UNI_FVEC3,
    UNI_FVEC4,
    UNI_DVEC1,
    UNI_DVEC2,
    UNI_DVEC3,
    UNI_DVEC4,
    UNI_IVEC1,
    UNI_IVEC2,
    UNI_IVEC3,
    UNI_IVEC4,
    UNI_UIVEC1,
    UNI_UIVEC2,
    UNI_UIVEC3,
    UNI_UIVEC4,
    UNI_FMAT2,
    UNI_FMAT23,
    UNI_FMAT24,
    UNI_FMAT3,
    UNI_FMAT32,
    UNI_FMAT34,
    UNI_FMAT4,
    UNI_FMAT42,
    UNI_FMAT43,
    UNI_DMAT2,
    UNI_DMAT23,
    UNI_DMAT24,
    UNI_DMAT3,
    UNI_DMAT32,
    UNI_DMAT34,
    UNI_DMAT4,
    UNI_DMAT42,
    UNI_DMAT43
};

struct Uniform
{
    std::string name;
    // UniformType uni_type;
    bool ismatrix;
    bool isvector;

    Uniform(/*UniformType unitype,*/ const char *namestr, bool isvec = false, bool ismat = false)
        : name(namestr)
        /* , uni_type(unitype) */
        , ismatrix(ismat), isvector(isvec)
    {}
    virtual ~Uniform() {}

    bool IsVector() { return isvector; }
    bool IsMatrix() { return ismatrix; }

    virtual int32 GetElementsCount() { return 0; }
    virtual void BindLocation(uint32 location) {}
};

struct UniformLocation
{
    Uniform* uniform;
    int32 location = -1;

    void Bind()
    {
        uniform->BindLocation(location);
    }
};


template<class T, uint32 N>
struct SimpleUniform;

template<class T>
struct SimpleUniform<T, 1> : public Uniform
{
    T u0;

    SimpleUniform(/*UniformType unitype,*/ const char *namestr)
        : Uniform(/*unitype,*/ namestr)
    {}
    virtual ~SimpleUniform() {}

    void Set(T v) { u0 = v; }
    virtual int32 GetElementsCount() override { return 1; }
    virtual void BindLocation(uint32 location) override;
};
template<class T>
struct SimpleUniform<T, 2> : public Uniform
{
    T u0, u1;

    SimpleUniform(/*UniformType unitype,*/ const char *namestr)
        : Uniform(/*unitype,*/ namestr)
    {}
    virtual ~SimpleUniform() {}

    void Set(T v0, T v1) { u0 = v0; u0 = v1; }
    virtual int32 GetElementsCount() override { return 2; }
    virtual void BindLocation(uint32 location) override;
};
template<class T>
struct SimpleUniform<T, 3> : public Uniform
{
    T u0, u1, u2;

    SimpleUniform(/*UniformType unitype,*/ const char *namestr)
        : Uniform(/*unitype,*/ namestr)
    {}
    virtual ~SimpleUniform() {}

    void Set(T v0, T v1, T v2) { u0 = v0; u0 = v1; u2 = v2; }

    virtual int32 GetElementsCount() override { return 3; }
    virtual void BindLocation(uint32 location) override;
};
template<class T>
struct SimpleUniform<T, 4> : public Uniform
{
    T u0, u1, u2, u3;

    SimpleUniform(/*UniformType unitype,*/ const char *namestr)
        : Uniform(/*unitype,*/ namestr)
    {}
    virtual ~SimpleUniform() {}

    void Set(T v0, T v1, T v2, T v3) { u0 = v0; u0 = v1; u2 = v2, u3 = v3; }

    virtual int32 GetElementsCount() override { return 4; }
    virtual void BindLocation(uint32 location) override;
};



template<class T, uint32 N>
struct VecUniform : public Uniform
{
    vec<N, T, defaultp> vector;

    VecUniform(/*UniformType unitype,*/ const char *namestr)
        : Uniform(/*unitype,*/ namestr, true, false)
    {}
    virtual ~VecUniform() {}

    void Set(vec<N, T, defaultp>& vec) { vector = vec; }

    virtual int32 GetElementsCount() override { return 1; }
    virtual void BindLocation(uint32 location) override;
};



template<class T, uint32 Row, uint32 Column>
struct MatUniform : public Uniform
{
    mat<Row, Column, T, defaultp> matrix;
    bool transpose;

    MatUniform(/*UniformType unitype,*/ const char *namestr)
        : Uniform(/*unitype,*/ namestr, false, true)
    {}
    virtual ~MatUniform() {}

    void Set(mat<Row, Column, T, defaultp>& mat, bool transposed = false) 
    { 
        matrix = mat;
        transpose = transposed; 
    }

    virtual int32 GetElementsCount() override { return 1; }
    virtual void BindLocation(uint32 location) override;
};





/*------------------------------Vertex Array Object--------------------------*/

struct VertexArrayObject
{

};



/*------------------------------Vertex Buffer--------------------------*/

struct VertexBuffer
{

};


/*------------------------------Index Buffer--------------------------*/

struct IndexBuffer
{

};



/*------------------------------Render Context--------------------------*/

/**
 * @brief Manage render resource.
 * 
 */
class RenderContext
{
public:
    RenderContext();
    ~RenderContext();


};


} // namespace TwinkleGraphics


#endif