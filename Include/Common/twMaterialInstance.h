#ifndef TW_MATERIALINSTANCE_H
#define TW_MATERIALINSTANCE_H

#include "twMaterial.h"

namespace TwinkleGraphics
{
class __TWCOMExport StandardMaterial : public Material
{
public:
    typedef std::shared_ptr<StandardMaterial> Ptr;

    StandardMaterial(ShaderOption* options, int32 num)
        : Material()
    {
        Initialize(options, num);
    }
    StandardMaterial(const StandardMaterial &copy)
        : Material(copy)
    {
    }
    virtual ~StandardMaterial() {}

private:
    void Initialize(ShaderOption options[], int32 num)
    {
        ShaderManager& shaderMgr = ShaderMgrInstance();

        ShaderProgramOption programOption(options, num);
        programOption.AddSuccessFunc(-1, this, &StandardMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, num);
    }

    void OnMaterialSuccess(Object::Ptr obj)
    {
        if(obj == nullptr)
            return;

        ShaderProgram::Ptr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
        if(program != nullptr)
        {
            RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
            this->AddRenderPass(pass);
            this->ApplyPassUniforms();

            vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
            this->SetVecUniformValue<float32, 4>("tintColor", tintColor);

            this->SetValid(true);
        }
    }
};

class __TWCOMExport BasicGeomMaterial : public Material
{
public:
    typedef std::shared_ptr<BasicGeomMaterial> Ptr;

    BasicGeomMaterial()
        : Material()
    {
        Initialize();
    }
    BasicGeomMaterial(const BasicGeomMaterial &copy)
        : Material(copy)
    {
    }
    virtual ~BasicGeomMaterial() {}

private:
    void Initialize()
    {
        ShaderManager& shaderMgr = ShaderMgrInstance();
         
        ShaderOption options[] = { 
            ShaderOption::OptionData{std::string("Assets/Shaders/basicGeometry.vert"), ShaderType::VERTEX_SHADER},
            ShaderOption::OptionData{std::string("Assets/Shaders/basicGeometry.frag"), ShaderType::FRAGMENT_SHADER}
        };

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &BasicGeomMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    void OnMaterialSuccess(Object::Ptr obj)
    {
        if(obj == nullptr)
            return;

        ShaderProgram::Ptr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
        if(program != nullptr)
        {
            RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
            this->AddRenderPass(pass);
            this->ApplyPassUniforms();

            vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
            this->SetVecUniformValue<float32, 4>("tintColor", tintColor);

            this->SetValid(true);
        }
    }    
};

class __TWCOMExport SpriteMaterial : public Material
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
        ShaderManager& shaderMgr = ShaderMgrInstance();
 
        char* vertMacros[1] = { const_cast<char*>(VertexLayoutDefines[3]) };
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/sprite.vert"), ShaderType::VERTEX_SHADER, 1, vertMacros},
            ShaderOption::OptionData{std::string("Assets/Shaders/sprite.frag"), ShaderType::FRAGMENT_SHADER}};

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &SpriteMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    void OnMaterialSuccess(Object::Ptr obj)
    {
        if(obj == nullptr)
            return;

        ShaderProgram::Ptr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
        if(program != nullptr)
        {
            RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
            this->AddRenderPass(pass);
            this->ApplyPassUniforms();

            vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
            bvec2 flip(false, false);
            this->SetVecUniformValue<bool, 2>("flip", flip);
            this->SetVecUniformValue<float32, 4>("tintColor", tintColor);
            vec2 tiling(1.0f, 1.0f), offset(0.0f, 0.0f);
            this->SetTextureTiling("mainTex", tiling);
            this->SetTextureOffset("mainTex", offset);

            this->SetValid(true);
        }
    }       
};

class __TWCOMExport Sprite1DMaterial : public Material
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
        ShaderManager& shaderMgr = ShaderMgrInstance();
 
        char* vertMacros[1] = { const_cast<char*>(VertexLayoutDefines[3]) };
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/sprite.vert"), ShaderType::VERTEX_SHADER, 1, vertMacros},
            ShaderOption::OptionData{std::string("Assets/Shaders/sprite1D.frag"), ShaderType::FRAGMENT_SHADER}};

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &Sprite1DMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    void OnMaterialSuccess(Object::Ptr obj)
    {
        if(obj == nullptr)
            return;

        ShaderProgram::Ptr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
        if(program != nullptr)
        {
            RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
            this->AddRenderPass(pass);
            this->ApplyPassUniforms();

            vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
            bvec2 flip(false, false);
            this->SetVecUniformValue<bool, 2>("flip", flip);
            this->SetVecUniformValue<float32, 4>("tintColor", tintColor);
            vec2 tiling(1.0f, 1.0f), offset(0.0f, 0.0f);
            this->SetTextureTiling("mainTex", tiling);
            this->SetTextureOffset("mainTex", offset);

            this->SetValid(true);
        }
    }   
};

class __TWCOMExport LineMaterial : public Material
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
        ShaderManager& shaderMgr = ShaderMgrInstance();
 
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/line.vert"), ShaderType::VERTEX_SHADER},
            ShaderOption::OptionData{std::string("Assets/Shaders/line.geom"), ShaderType::GEOMETRY_SHADER},
            ShaderOption::OptionData{std::string("Assets/Shaders/line.frag"), ShaderType::FRAGMENT_SHADER}
        };

        ShaderProgramOption programOption(options, 3);
        programOption.AddSuccessFunc(-1, this, &LineMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 3);
    }

    void OnMaterialSuccess(Object::Ptr obj)
    {
        if(obj == nullptr)
            return;

        ShaderProgram::Ptr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
        if(program != nullptr)
        {
            RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
            this->AddRenderPass(pass);
            this->ApplyPassUniforms();

            this->SetValid(true);
        }
    }       
};

class __TWCOMExport InfinitePlaneMaterial : public Material
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
        ShaderManager& shaderMgr = ShaderMgrInstance();
 
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/infinitePlane.vert"), ShaderType::VERTEX_SHADER},
            ShaderOption::OptionData{std::string("Assets/Shaders/infinitePlane.frag"), ShaderType::FRAGMENT_SHADER}};

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &InfinitePlaneMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    void OnMaterialSuccess(Object::Ptr obj)
    {
        if(obj == nullptr)
            return;

        ShaderProgram::Ptr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
        if(program != nullptr)
        {
            RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
            this->AddRenderPass(pass);
            this->ApplyPassUniforms();

            this->SetValid(true);
        }
    }      
};

class __TWCOMExport VolumnQuadMaterial : public Material
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
        ShaderManager& shaderMgr = ShaderMgrInstance();
 
        char* vertMacros[1] = { const_cast<char*>(VertexLayoutDefines[3]) };
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/volumnQuad.vert"), ShaderType::VERTEX_SHADER, 1, vertMacros},
            ShaderOption::OptionData{std::string("Assets/Shaders/volumnQuad.frag"), ShaderType::FRAGMENT_SHADER}};

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &VolumnQuadMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }    

    void OnMaterialSuccess(Object::Ptr obj)
    {
        if(obj == nullptr)
            return;

        ShaderProgram::Ptr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
        if(program != nullptr)
        {
            RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
            this->AddRenderPass(pass);
            this->ApplyPassUniforms();

            vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
            bvec2 flip(false, false);
            this->SetVecUniformValue<bool, 2>("flip", flip);
            this->SetVecUniformValue<float32, 4>("tintColor", tintColor);
            vec2 tiling(1.0f, 1.0f), offset(0.0f, 0.0f);
            this->SetTextureTiling("mainTex", tiling);
            this->SetTextureOffset("mainTex", offset);    

            this->SetValid(true);
        }
    }        
};


class __TWCOMExport CubeMaterial : public Material
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
        ShaderManager& shaderMgr = ShaderMgrInstance();
 
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/cube.vert"), ShaderType::VERTEX_SHADER},
            ShaderOption::OptionData{std::string("Assets/Shaders/cube.frag"), ShaderType::FRAGMENT_SHADER}};

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &CubeMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    void OnMaterialSuccess(Object::Ptr obj)
    {
        if(obj == nullptr)
            return;

        ShaderProgram::Ptr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
        if(program != nullptr)
        {
            RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
            this->AddRenderPass(pass);
            this->ApplyPassUniforms();

            vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
            this->SetVecUniformValue<float32, 4>("tintColor", tintColor);  

            this->SetValid(true);
        }
    }       
};


class __TWCOMExport SphereMaterial : public Material
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
        ShaderManager& shaderMgr = ShaderMgrInstance();
 
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/sphere.vert"), ShaderType::VERTEX_SHADER},
            ShaderOption::OptionData{std::string("Assets/Shaders/sphere.frag"), ShaderType::FRAGMENT_SHADER}};

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &SphereMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    void OnMaterialSuccess(Object::Ptr obj)
    {
        if(obj == nullptr)
            return;

        ShaderProgram::Ptr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
        if(program != nullptr)
        {
            RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
            this->AddRenderPass(pass);
            this->ApplyPassUniforms();

            vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
            this->SetVecUniformValue<float32, 4>("tintColor", tintColor);     

            this->SetValid(true);
        }
    }
};


class __TWCOMExport SkyboxMaterial : public Material
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
        ShaderManager& shaderMgr = ShaderMgrInstance();
 
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/skybox.vert"), ShaderType::VERTEX_SHADER},
            ShaderOption::OptionData{std::string("Assets/Shaders/skybox.frag"), ShaderType::FRAGMENT_SHADER}};

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &SkyboxMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    void OnMaterialSuccess(Object::Ptr obj)
    {
        if(obj == nullptr)
            return;

        ShaderProgram::Ptr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
        if(program != nullptr)
        {
            RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
            this->AddRenderPass(pass);
            this->ApplyPassUniforms();

            vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
            bvec2 flip(false, false);
            this->SetVecUniformValue<bool, 2>("flip", flip);
            this->SetVecUniformValue<float32, 4>("tintColor", tintColor);
            vec2 tiling(1.0f, 1.0f), offset(0.0f, 0.0f);
            this->SetTextureTiling("mainTex", tiling);
            this->SetTextureOffset("mainTex", offset);

            this->SetValid(true);
        }
    }       
};

class __TWCOMExport ProjectionMappingMaterial : public Material
{
public:

    typedef std::shared_ptr<ProjectionMappingMaterial> Ptr;

    ProjectionMappingMaterial()
        : Material()
    {
        Initialize();
    }
    ProjectionMappingMaterial(const ProjectionMappingMaterial &copy)
        : Material(copy)
    {
    }
    virtual ~ProjectionMappingMaterial() {}

private:
    void Initialize()
    {
        ShaderManager& shaderMgr = ShaderMgrInstance();
 
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/projectionTexture.vert"), ShaderType::VERTEX_SHADER},
            ShaderOption::OptionData{std::string("Assets/Shaders/projectionTexture.frag"), ShaderType::FRAGMENT_SHADER}};

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &ProjectionMappingMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    void OnMaterialSuccess(Object::Ptr obj)
    {
        if(obj == nullptr)
            return;

        ShaderProgram::Ptr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
        if(program != nullptr)
        {
            RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
            this->AddRenderPass(pass);
            this->ApplyPassUniforms();

            vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
            this->SetVecUniformValue<float32, 4>("tintColor", tintColor);

            this->SetValid(true);
        }
    }     
};

class __TWCOMExport ScreenQuadMaterial : public Material
{
public:

    typedef std::shared_ptr<ScreenQuadMaterial> Ptr;

    ScreenQuadMaterial()
        : Material()
    {
        Initialize();
    }
    ScreenQuadMaterial(const ScreenQuadMaterial &copy)
        : Material(copy)
    {
    }
    virtual ~ScreenQuadMaterial() {}

private:
    void Initialize()
    {
        ShaderManager& shaderMgr = ShaderMgrInstance();

        char* vertMacros[1] = { const_cast<char*>(VertexLayoutDefines[3]) };
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/screenquad.vert"), ShaderType::VERTEX_SHADER, 1, vertMacros},
            ShaderOption::OptionData{std::string("Assets/Shaders/screenquad.frag"), ShaderType::FRAGMENT_SHADER}};

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &ScreenQuadMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    void OnMaterialSuccess(Object::Ptr obj)
    {
        if(obj == nullptr)
            return;

        ShaderProgram::Ptr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
        if(program != nullptr)
        {
            RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
            this->AddRenderPass(pass);
            this->ApplyPassUniforms();

            this->SetValid(true);
        }
    }     
};

class __TWCOMExport MSAAResolveMaterial : public Material
{
public:

    typedef std::shared_ptr<MSAAResolveMaterial> Ptr;

    MSAAResolveMaterial()
        : Material()
    {
        Initialize();
    }
    MSAAResolveMaterial(const MSAAResolveMaterial &copy)
        : Material(copy)
    {
    }
    virtual ~MSAAResolveMaterial() {}

private:
    void Initialize()
    {
        ShaderManager& shaderMgr = ShaderMgrInstance();

        char* vertMacros[1] = { const_cast<char*>(VertexLayoutDefines[3]) };
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/screenquad.vert"), ShaderType::VERTEX_SHADER, 1, vertMacros},
            ShaderOption::OptionData{std::string("Assets/Shaders/msaaResolve.frag"), ShaderType::FRAGMENT_SHADER}};

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &MSAAResolveMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    void OnMaterialSuccess(Object::Ptr obj)
    {
        if(obj == nullptr)
            return;

        ShaderProgram::Ptr program = std::dynamic_pointer_cast<ShaderProgram>(obj);
        if(program != nullptr)
        {
            RenderPass::Ptr pass = std::make_shared<RenderPass>(program);
            this->AddRenderPass(pass);
            this->ApplyPassUniforms();

            this->SetValid(true);
        }
    }     
};

} // namespace TwinkleGraphics

#endif