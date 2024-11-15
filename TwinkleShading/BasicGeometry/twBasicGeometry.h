
#ifndef TW_BASICGEOMETRY_H
#define TW_BASICGEOMETRY_H

#include "twCommon.h"

#include "twCamera.h"
#include "twFirstPersonControl.h"
#include "twGeometry.h"
#include "twView.h"
#include "twGLViewPlugin.h"
#include "twShader.h"


namespace TwinkleGraphics {
class BasicGeometryView;

class BasicGeometry : public GLViewPlugin {
public:
  BasicGeometry(std::string &name);
  ~BasicGeometry();

  virtual void Install() override;
  virtual void UnInstall() override;

private:
  BasicGeometryView *_view;
};

class BasicGeometryView : public View {
public:
  BasicGeometryView()
      : View(), _linePoints(nullptr), _program(nullptr), _lineProgram(nullptr),
        _uvsphere(nullptr), _norcubesphere(nullptr), _icosphere(nullptr),
        _cube(nullptr), _quad(nullptr), _line(nullptr), _infinitePlane(nullptr),
        _currentMesh(nullptr), _quadbezierline(nullptr),
        _cubicbezierline(nullptr), _bspline(nullptr), _nurbsSurface(nullptr),
        _nurbsControlLine(nullptr), _currentMeshIndex(-1), _sphereRadius(5.0f),
        _uvsphereLongitude(50), _uvsphereLatitude(25), _sphereSubdivide(10),
        _lineAntialiasing(false), _lineIsdashed(false), _lineType(false),
        _displayInfiniteplane(false), _polygonLineMode(true), _cullFace(true) {}
  virtual ~BasicGeometryView() {}

protected:
  virtual void Initialized() override;
  virtual void Destroy() override;
  virtual void Advance(float64 delta_time) override;
  virtual void RenderImpl() override;
  virtual void OnGUI() override;

private:
  enum class LineJointType { BEVEL, ROUND, MITER };

  enum class LineCapType { BUTT, ROUND, SQUARE };

  void CreateUVSphere();
  void CreateNorCubeSphere();
  void CreateIcoSphere();
  void CreateCube();
  void CreateQuad();
  void CreateLine();
  void CreateInfinitePlane();
  void CreateQuadBezierLine();
  void CreateCubicBezierLine();
  void CreateBSpline();
  void CreateNURBS();
  void CreateBezierSuface();
  void CreateNURBSSurface();

  void RenderUVSphere();
  void RenderNorCubeSphere();
  void RenderIcoSphere();
  void RenderCube();
  void RenderQuad();

  void CreateGeometry(MeshPtr mesh, uint32 index);

  void RenderGeometry(MeshPtr mesh, int32 index, GLenum front_face = GL_CCW);
  void RenderInfinitePlane();
  void RenderLine(MeshPtr mesh, int32 index = 5);

  void OnParametersGUI();

private:
  glm::mat4 _modelMat;
  glm::mat4 _viewMat;
  glm::mat4 _projectionMat;
  glm::mat4 _mvpMat;

  glm::vec4 _lineParams;
  glm::vec4 _viewportParams;
  glm::vec3 _lineColor;

  uint32 *_vaos;
  uint32 *_vbos;
  uint32 *_ebos;

  glm::vec3 *_linePoints;

  ShaderProgramPtr _program;
  ShaderProgramPtr _lineProgram;

  MeshPtr _uvsphere;
  MeshPtr _norcubesphere;
  MeshPtr _icosphere;
  MeshPtr _cube;
  MeshPtr _quad;
  MeshPtr _line;
  MeshPtr _currentMesh;
  MeshPtr _quadbezierline;
  MeshPtr _cubicbezierline;
  BSplineCurvePtr _bspline;
  NURBSSurfacePtr _nurbsSurface;
  MeshPtr _nurbsControlLine;
  PlanePtr _infinitePlane;

  uint32 _mvpMatLoc;

  uint32 _lineMVPLoc;
  uint32 _lineParametersLoc; // x:thickness, y:feather, z:miterlimit, w:aspect
  uint32 _lineColorLoc;
  uint32 _viewportLoc;

  GLenum _frontFace;
  int32 _currentMeshIndex;

  float32 _sphereRadius;
  int32 _uvsphereLongitude;
  int32 _uvsphereLatitude;
  int32 _sphereSubdivide;

  bool _lineAntialiasing;
  bool _lineIsdashed;
  bool _lineType;
  bool _displayInfiniteplane;
  bool _polygonLineMode;
  bool _cullFace;

  friend class BasicGeometry;
};

} // namespace TwinkleGraphics

#endif