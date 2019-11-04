#ifndef TW_MATERIALINSTANCE_H
#define TW_MATERIALINSTANCE_H

#include "twMaterial.h"

namespace TwinkleGraphics
{
class BasicGeomMaterial : public Material
{
public:
    typedef std::shared_ptr<BasicGeomMaterial> Ptr;

    BasicGeomMaterial()
        : Material()
    {
    }
    BasicGeomMaterial(const BasicGeomMaterial &copy)
        : Material(copy)
    {
    }
    virtual ~BasicGeomMaterial() {}

private:
    void Initialize()
    {
        ShaderManagerInst shaderMgr;
        ShaderProgram::Ptr program = nullptr;
        ShaderReadInfo shader_info[] = {
            {std::string("Assets/Shaders/basic_geometry.vert"), ShaderType::VERTEX_SHADER},
            {std::string("Assets/Shaders/basic_geometry.frag"), ShaderType::FRAGMENT_SHADER}};
        program = shaderMgr->ReadShaders(shader_info, 2);

        RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
        this->AddRenderPass(pass);
    }
};

class SpriteMaterial : public Material
{
public:
    typedef std::shared_ptr<SpriteMaterial> Ptr;

    SpriteMaterial()
        : Material()
    {
        Initialize();
    }
    SpriteMaterial(const SpriteMaterial &copy)
        : Material(copy)
    {
    }
    virtual ~SpriteMaterial() {}

private:
    void Initialize()
    {
        ShaderManagerInst shaderMgr;
        ShaderProgram::Ptr program = nullptr;
        ShaderReadInfo shader_info[] = {
            {std::string("Assets/Shaders/sprite.vert"), ShaderType::VERTEX_SHADER},
            {std::string("Assets/Shaders/sprite.frag"), ShaderType::FRAGMENT_SHADER}};
        program = shaderMgr->ReadShaders(shader_info, 2);

        RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
        this->AddRenderPass(pass);

        vec4 tint_color(1.0f, 1.0f, 1.0f, 1.0f);
        bvec2 flip(false, false);
        this->SetVecUniformValue<bool, 2>("flip", flip);
        this->SetVecUniformValue<float32, 4>("tint_color", tint_color);
        vec2 tiling(1.0f, 1.0f), offset(0.0f, 0.0f);
        this->SetTextureTiling("main_tex", tiling);
        this->SetTextureOffset("main_tex", offset);
    }
};

class Sprite1DMaterial : public Material
{
public:
    typedef std::shared_ptr<Sprite1DMaterial> Ptr;

    Sprite1DMaterial()
        : Material()
    {
        Initialize();
    }
    Sprite1DMaterial(const Sprite1DMaterial &copy)
        : Material(copy)
    {
    }
    virtual ~Sprite1DMaterial() {}

private:
    void Initialize()
    {
        ShaderManagerInst shaderMgr;
        ShaderProgram::Ptr program = nullptr;
        ShaderReadInfo shader_info[] = {
            {std::string("Assets/Shaders/sprite.vert"), ShaderType::VERTEX_SHADER},
            {std::string("Assets/Shaders/sprite_1d.frag"), ShaderType::FRAGMENT_SHADER}};
        program = shaderMgr->ReadShaders(shader_info, 2);

        RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
        this->AddRenderPass(pass);

        vec4 tint_color(1.0f, 1.0f, 1.0f, 1.0f);
        bvec2 flip(false, false);
        this->SetVecUniformValue<bool, 2>("flip", flip);
        this->SetVecUniformValue<float32, 4>("tint_color", tint_color);
        vec2 tiling(1.0f, 1.0f), offset(0.0f, 0.0f);
        this->SetTextureTiling("main_tex", tiling);
        this->SetTextureOffset("main_tex", offset);
    }
};

class LineMaterial : public Material
{
public:
    typedef std::shared_ptr<LineMaterial> Ptr;

    LineMaterial()
        : Material()
    {
        Initialize();
    }
    LineMaterial(const LineMaterial &copy)
        : Material(copy)
    {
    }
    virtual ~LineMaterial() {}

private:
    void Initialize()
    {
        ShaderManagerInst shaderMgr;
        ShaderProgram::Ptr program = nullptr;
        ShaderReadInfo shader_info[] = {
            {std::string("Assets/Shaders/line.vert"), ShaderType::VERTEX_SHADER},
            {std::string("Assets/Shaders/line.geom"), ShaderType::GEOMETRY_SHADER},
            {std::string("Assets/Shaders/line.frag"), ShaderType::FRAGMENT_SHADER}};
        program = shaderMgr->ReadShaders(shader_info, 3);

        RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
        this->AddRenderPass(pass);
    }
};

class InfinitePlaneMaterial : public Material
{
public:
    typedef std::shared_ptr<InfinitePlaneMaterial> Ptr;

    InfinitePlaneMaterial()
        : Material()
    {
        Initialize();
    }
    InfinitePlaneMaterial(const InfinitePlaneMaterial &copy)
        : Material(copy)
    {
    }
    virtual ~InfinitePlaneMaterial() {}

private:
    void Initialize()
    {
        ShaderManagerInst shaderMgr;
        ShaderProgram::Ptr program = nullptr;
        ShaderReadInfo shader_info[] = {
            {std::string("Assets/Shaders/infinite_plane.vert"), ShaderType::VERTEX_SHADER},
            {std::string("Assets/Shaders/infinite_plane.frag"), ShaderType::FRAGMENT_SHADER}};
        program = shaderMgr->ReadShaders(shader_info, 2);

        RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
        this->AddRenderPass(pass);
    }
};

class VolumnQuadMaterial : public Material
{
public:
    typedef std::shared_ptr<VolumnQuadMaterial> Ptr;

    VolumnQuadMaterial()
        : Material()
    {
        Initialize();
    }
    VolumnQuadMaterial(const VolumnQuadMaterial &copy)
        : Material(copy)
    {
    }
    virtual ~VolumnQuadMaterial() {}

private:
    void Initialize()
    {
        ShaderManagerInst shaderMgr;
        ShaderProgram::Ptr program = nullptr;
        ShaderReadInfo shader_info[] = {
            {std::string("Assets/Shaders/volumn_quad.vert"), ShaderType::VERTEX_SHADER},
            {std::string("Assets/Shaders/volumn_quad.frag"), ShaderType::FRAGMENT_SHADER}};
        program = shaderMgr->ReadShaders(shader_info, 2);

        RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
        this->AddRenderPass(pass);

        vec4 tint_color(1.0f, 1.0f, 1.0f, 1.0f);
        bvec2 flip(false, false);
        this->SetVecUniformValue<bool, 2>("flip", flip);
        this->SetVecUniformValue<float32, 4>("tint_color", tint_color);
        vec2 tiling(1.0f, 1.0f), offset(0.0f, 0.0f);
        this->SetTextureTiling("main_tex", tiling);
        this->SetTextureOffset("main_tex", offset);
    }    
};


class CubeMaterial : public Material
{
public:
    typedef std::shared_ptr<CubeMaterial> Ptr;

    CubeMaterial()
        : Material()
    {
        Initialize();
    }
    CubeMaterial(const CubeMaterial &copy)
        : Material(copy)
    {
    }
    virtual ~CubeMaterial() {}

private:
    void Initialize()
    {
        ShaderManagerInst shaderMgr;
        ShaderProgram::Ptr program = nullptr;
        ShaderReadInfo shader_info[] = {
            {std::string("Assets/Shaders/cube.vert"), ShaderType::VERTEX_SHADER},
            {std::string("Assets/Shaders/cube.frag"), ShaderType::FRAGMENT_SHADER}};
        program = shaderMgr->ReadShaders(shader_info, 2);

        RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
        this->AddRenderPass(pass);

        vec4 tint_color(1.0f, 1.0f, 1.0f, 1.0f);
        this->SetVecUniformValue<float32, 4>("tint_color", tint_color);
    }
};


class SphereMaterial : public Material
{
public:
    typedef std::shared_ptr<SphereMaterial> Ptr;

    SphereMaterial()
        : Material()
    {
        Initialize();
    }
    SphereMaterial(const SphereMaterial &copy)
        : Material(copy)
    {
    }
    virtual ~SphereMaterial() {}

private:
    void Initialize()
    {
        ShaderManagerInst shaderMgr;
        ShaderProgram::Ptr program = nullptr;
        ShaderReadInfo shader_info[] = {
            {std::string("Assets/Shaders/sphere.vert"), ShaderType::VERTEX_SHADER},
            {std::string("Assets/Shaders/sphere.frag"), ShaderType::FRAGMENT_SHADER}};
        program = shaderMgr->ReadShaders(shader_info, 2);

        RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
        this->AddRenderPass(pass);

        vec4 tint_color(1.0f, 1.0f, 1.0f, 1.0f);
        this->SetVecUniformValue<float32, 4>("tint_color", tint_color);
    }
};


class SkyboxMaterial : public Material
{
public:
    typedef std::shared_ptr<SkyboxMaterial> Ptr;

    SkyboxMaterial()
        : Material()
    {
        Initialize();
    }
    SkyboxMaterial(const SkyboxMaterial &copy)
        : Material(copy)
    {
    }
    virtual ~SkyboxMaterial() {}

private:
    void Initialize()
    {
        ShaderManagerInst shaderMgr;
        ShaderProgram::Ptr program = nullptr;
        ShaderReadInfo shader_info[] = {
            {std::string("Assets/Shaders/skybox.vert"), ShaderType::VERTEX_SHADER},
            {std::string("Assets/Shaders/skybox.frag"), ShaderType::FRAGMENT_SHADER}};
        program = shaderMgr->ReadShaders(shader_info, 2);

        RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
        this->AddRenderPass(pass);

        vec4 tint_color(1.0f, 1.0f, 1.0f, 1.0f);
        bvec2 flip(false, false);
        this->SetVecUniformValue<bool, 2>("flip", flip);
        this->SetVecUniformValue<float32, 4>("tint_color", tint_color);
        vec2 tiling(1.0f, 1.0f), offset(0.0f, 0.0f);
        this->SetTextureTiling("main_tex", tiling);
        this->SetTextureOffset("main_tex", offset);        
    }
};

} // namespace TwinkleGraphics

#endif