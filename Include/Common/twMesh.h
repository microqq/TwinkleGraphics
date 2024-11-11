

#ifndef TW_MESH_H
#define TW_MESH_H

#include <iostream>
#include <vector>

#include "twCommon.h"
#include "twRenderContext.h"

namespace TwinkleGraphics {

using MeshDataFlag = VertexLayoutFlag;
// enum MeshDataFlag
// {
//     DEFAULT = 0,
//     HAS_COLOR = 1,
//     HAS_BINORMAL = 2,
//     HAS_NORMAL = 4,
//     HAS_UV = 8,
//     HAS_UV1 = 16,
//     HAS_UV2 = 32,
//     HAS_UV3 = 64,
//     HAS_UV4 = 128,
//     HAS_UV5 = 256,
//     HAS_UV6 = 512,
//     HAS_UV7 = 1024
// };

enum class LineJointFlag { MITER, BEVEL, ROUND };

enum class LineEndCapFlag { BUTT, ROUND, SQUARE };

class SubMesh;
class Mesh;
class Triangle;
class Line;
class Quad;
class Plane;
class Cube;
class UVSphere;
class NormalizedCubeSphere;
class IcosahedronSphere;
class BezierCurve;
// template<>
// class BezierCurve<2>;
// template<>
// class BezierCurve<3>;
class BSplineCurve;
class NURBSCurve;
class BezierSurface;
class NURBSSurface;

class __TWAPI SubMesh : public Object {
public:
  using Ptr = std::shared_ptr<SubMesh>;
  using WeakPtr = std::weak_ptr<SubMesh>;

  SubMesh() : _indice(nullptr), _indiceNum(0) {}
  SubMesh(const SubMesh &copy) {}
  virtual ~SubMesh() { SAFE_DEL(_indice); }

  void Initialize(int32 indice_num);
  uint32 *GetIndice() { return _indice; }
  int32 GetIndiceNum() { return _indiceNum; }
  void SetMaterialIndex(int32 index) { _materialIndex = index; }
  int32 GetMaterialIndex() { return _materialIndex; }

private:
  uint32 *_indice;
  int32 _indiceNum;
  int32 _materialIndex = 0;

  friend class Mesh;
  friend class Triangle;
  friend class Line;
  friend class Quad;
  friend class Plane;
  friend class Cube;
  friend class UVSphere;
  friend class NormalizedCubeSphere;
  friend class IcosahedronSphere;
  friend class BezierCurve;
  friend class BSplineCurve;
  friend class NURBSCurve;
  friend class BezierSurface;
  friend class NURBSSurface;
};

using SubMeshPtr = SubMesh::Ptr;

class __TWAPI Mesh : public Object {
public:
  using Ptr = std::shared_ptr<Mesh>;
  using WeakPtr = std::weak_ptr<Mesh>;

  /*=============deprecated, use "Geometry" instead=============*/
  static Ptr CreateLineMesh(glm::vec3 *points, int32 num);
  /*=============deprecated, use "Geometry" instead=============*/
  static Ptr CreateLineMeshEx(glm::vec4 *points, int32 num);
  /*=============deprecated, use "Geometry" instead=============*/
  static Ptr CreateQuadMesh(float32 x, float32 y);
  /*=============deprecated, use "Geometry" instead=============*/
  static Ptr CreateCubeMesh(float32 size);
  /*=============deprecated, use "Geometry" instead=============*/
  static Ptr CreateSphereMeshStandard(float32 radius, int32 longitude_count,
                                      int32 latitude_count);
  /*=============deprecated, use "Geometry" instead=============*/
  static Ptr CreateSphereMeshNormalizedCube(float32 radius, int32 subdivide);
  /*=============deprecated, use "Geometry" instead=============*/
  static Ptr CreateSphereMeshIcosahedron(float32 radius, int32 subdivide);
  /*=============deprecated, use "Geometry" instead=============*/
  static Ptr CreateBezierLine(glm::vec3 *points, int32 num,
                              int32 segments = 128);
  /*=============deprecated, use "Geometry" instead=============*/
  static Ptr CreateQuadraticBezierLine(glm::vec3 *points, int32 segments = 128);
  /*=============deprecated, use "Geometry" instead=============*/
  static Ptr CreateCubicBezierLine(glm::vec3 *points, int32 segments = 128);
  /*=============deprecated, use "Geometry" instead=============*/

private:
  /*=============deprecated, use "Geometry" instead=============*/
  static void CreateIconsahedron(glm::vec3 *vertice, uint32 *indice,
                                 float32 radius);
  /*=============deprecated, use "Geometry" instead=============*/
  static glm::vec3 DeCasteljau(glm::vec3 *points, glm::vec3 *helper, int32 num,
                               float32 u);
  /*=============deprecated=============*/

public:
  Mesh()
      : _verticePos(nullptr), _verticeColor(nullptr), _verticeBinormal(nullptr),
        _verticeNormal(nullptr), _verticeUV(nullptr), _verticeUV1(nullptr),
        _verticeUV2(nullptr), _verticeUV3(nullptr), _verticeUV4(nullptr),
        _verticeUV5(nullptr), _verticeUV6(nullptr), _verticeUV7(nullptr),
        _dataFlag(MeshDataFlag::DEFAULT), _verticeNum(0) {}
  virtual ~Mesh();

  void Initialize(int32 num, MeshDataFlag flag = MeshDataFlag(13)) {
    if (num <= 0)
      return;

    _dataFlag = flag;

    SAFE_DEL_ARR(_verticePos);
    _verticePos = new glm::vec3[num];
    _verticeNum = num;

    if (flag == MeshDataFlag::DEFAULT)
      return;

    if ((flag & MeshDataFlag::HAS_COLOR) != 0) {
      SAFE_DEL_ARR(_verticeColor);
      _verticeColor = new glm::vec4[num];
    }

    if ((flag & MeshDataFlag::HAS_BINORMAL) != 0) {
      SAFE_DEL_ARR(_verticeBinormal);
      _verticeBinormal = new glm::vec3[num];
    }

    if ((flag & MeshDataFlag::HAS_NORMAL) != 0) {
      SAFE_DEL_ARR(_verticeNormal);
      _verticeNormal = new glm::vec3[num];
    }

    if ((flag & MeshDataFlag::HAS_UV) != 0) {
      SAFE_DEL_ARR(_verticeUV);
      _verticeUV = new glm::vec4[num];
    }
    if ((flag & MeshDataFlag::HAS_UV1) != 0) {
      SAFE_DEL_ARR(_verticeUV1);
      _verticeUV1 = new glm::vec4[num];
    }
    if ((flag & MeshDataFlag::HAS_UV2) != 0) {
      SAFE_DEL_ARR(_verticeUV2);
      _verticeUV2 = new glm::vec4[num];
    }
    if ((flag & MeshDataFlag::HAS_UV3) != 0) {
      SAFE_DEL_ARR(_verticeUV3);
      _verticeUV3 = new glm::vec4[num];
    }
    if ((flag & MeshDataFlag::HAS_UV4) != 0) {
      SAFE_DEL_ARR(_verticeUV4);
      _verticeUV4 = new glm::vec4[num];
    }
    if ((flag & MeshDataFlag::HAS_UV5) != 0) {
      SAFE_DEL_ARR(_verticeUV5);
      _verticeUV5 = new glm::vec4[num];
    }
    if ((flag & MeshDataFlag::HAS_UV6) != 0) {
      SAFE_DEL_ARR(_verticeUV6);
      _verticeUV6 = new glm::vec4[num];
    }
    if ((flag & MeshDataFlag::HAS_UV7) != 0) {
      SAFE_DEL_ARR(_verticeUV7);
      _verticeUV7 = new glm::vec4[num];
    }
  }

  void AddSubMesh(SubMeshPtr submesh) { _submeshes.emplace_back(submesh); }
  void RemoveSubMesh(int32 index) {
    int32 size = _submeshes.size();
    if (index < size)
      _submeshes.erase(_submeshes.begin() + index);
  }
  inline uint32 GetSubMeshCount() { return _submeshes.size(); }

  SubMeshPtr GetSubMesh(int32 index) {
    int32 size = _submeshes.size();
    if (index < size)
      return _submeshes[index];
    return nullptr;
  }

  glm::vec3 *GetVerticePos() { return _verticePos; }
  int32 GetVerticeNum() { return _verticeNum; }
  glm::vec4 *GetVerticeColor() { return _verticeColor; }
  glm::vec4 *GetVerticeUV() { return _verticeUV; }
  glm::vec4 *GetVerticeUV1() { return _verticeUV1; }
  glm::vec4 *GetVerticeUV2() { return _verticeUV2; }
  glm::vec4 *GetVerticeUV3() { return _verticeUV3; }
  glm::vec4 *GetVerticeUV4() { return _verticeUV4; }
  glm::vec4 *GetVerticeUV5() { return _verticeUV5; }
  glm::vec4 *GetVerticeUV6() { return _verticeUV6; }
  glm::vec4 *GetVerticeUV7() { return _verticeUV7; }
  glm::vec3 *GetVerticeBiNormal() { return _verticeBinormal; }
  glm::vec3 *GetVerticeNormal() { return _verticeNormal; }

  int32 GetDataSize() {
    int32 totalsize = _verticeNum * 12;
    if ((_dataFlag & MeshDataFlag::HAS_COLOR) != 0) {
      totalsize += _verticeNum * 16;
    }
    if ((_dataFlag & MeshDataFlag::HAS_NORMAL) != 0) {
      totalsize += _verticeNum * 12;
    }
    if ((_dataFlag & MeshDataFlag::HAS_BINORMAL) != 0) {
      totalsize += _verticeNum * 12;
    }
    if ((_dataFlag & MeshDataFlag::HAS_UV) != 0) {
      totalsize += _verticeNum * 16;
    }
    int32 uvSize = _verticeNum * 16;
    int32 uvFlag = MeshDataFlag::HAS_UV7;
    int32 uvsNum = 0;
    for (unsigned int i = 0; i < 7; i++) {
      uvsNum += (_dataFlag & MeshDataFlag(uvFlag >> i)) ? 1 : 0;
    }
    totalsize += uvsNum * uvSize;

    return totalsize;
  }

  MeshDataFlag GetMeshDataFlag() { return _dataFlag; }

private:
  std::vector<SubMeshPtr> _submeshes;
  glm::vec3 *_verticePos;
  glm::vec4 *_verticeColor;
  glm::vec3 *_verticeBinormal;
  glm::vec3 *_verticeNormal;
  glm::vec4 *_verticeUV;
  glm::vec4 *_verticeUV1;
  glm::vec4 *_verticeUV2;
  glm::vec4 *_verticeUV3;
  glm::vec4 *_verticeUV4;
  glm::vec4 *_verticeUV5;
  glm::vec4 *_verticeUV6;
  glm::vec4 *_verticeUV7;

  MeshDataFlag _dataFlag;
  int32 _verticeNum;

  friend class Triangle;
  friend class Line;
  friend class Quad;
  friend class Plane;
  friend class Cube;
  friend class UVSphere;
  friend class NormalizedCubeSphere;
  friend class IcosahedronSphere;
  friend class BezierCurve;
  friend class BSplineCurve;
  friend class NURBSCurve;
  friend class BezierSurface;
  friend class NURBSSurface;
};

using MeshPtr = Mesh::Ptr;
} // namespace TwinkleGraphics

#endif