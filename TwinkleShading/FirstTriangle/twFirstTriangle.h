
#ifndef TW_FIRSTTRIANGLE_H
#define TW_FIRSTTRIANGLE_H

#include "twCommon.h"

#include "twPlugin.h"
#include "twShader.h"

namespace TwinkleGraphics
{
class TriangleView;

class FirstTriangle : public GLPlugin
{
public:
    FirstTriangle(std::string& name);
    ~FirstTriangle();

    virtual void Install() override;
    virtual void UnInstall() override;

private:
    TriangleView* _view;
};

class TriangleView : public View
{
public:
    TriangleView()
        : View()
        , _program(nullptr)
    {}
    virtual ~TriangleView()
    {}

protected:
    virtual void Initialize() override;
    virtual void Destroy() override;
    virtual void Advance(float64 delta_time) override;
    virtual void RenderImpl() override;
    virtual void OnGUI() override;
    
    friend class FirstTriangle;

private:
    uint32* _vaos;
    uint32* _vbos;
    uint32* _ebos;

    ShaderProgram::Ptr _program;

    uint32 _model_mat_loc;
    uint32 _view_mat_loc;
    uint32 _projection_mat_loc;

    glm::mat4 _model_mat;
    glm::mat4 _view_mat;
    glm::mat4 _projection_mat;
};

} // namespace TwinkleGraphics

#endif