
#ifndef TW_BASICGEOMETRY_H
#define TW_BASICGEOMETRY_H

#include "twCommon.h"

#include "twPlugin.h"
#include "twShader.h"
#include "twMesh.h"

namespace TwinkleGraphics
{
class BasicGeometryView;

class BasicGeometry : public GLPlugin
{
public:
    BasicGeometry(std::string& name);
    ~BasicGeometry();

    virtual void Install() override;
    virtual void UnInstall() override;

private:
    BasicGeometryView* _view;
};

class BasicGeometryView : public View
{
public:
    BasicGeometryView(Viewport& viewport)
        : View(viewport)
        , _program(nullptr)
    {}
    virtual ~BasicGeometryView()
    {}

protected:
    virtual void Initialize() override;
    virtual void Destroy() override;
    virtual void RenderImpl() override;
    virtual void OnGUI() override;
    
    friend class BasicGeometry;

private:
    uint32* _vaos;
    uint32* _vbos;
    uint32* _ebos;

    ShaderProgram::Ptr _program;
    Mesh::Ptr _uvsphere;

    uint32 _model_mat_loc;
    uint32 _view_mat_loc;
    uint32 _projection_mat_loc;

    glm::mat4 _model_mat;
    glm::mat4 _view_mat;
    glm::mat4 _projection_mat;
};

} // namespace TwinkleGraphics

#endif