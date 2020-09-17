#ifndef TW_MATERIALINSTANCE_H
#define TW_MATERIALINSTANCE_H

#include "twMaterial.h"

namespace TwinkleGraphics
{

class __TWCOMExport StandardMaterial : public Material
{
public:
    typedef std::shared_ptr<StandardMaterial> Ptr;

    StandardMaterial(std::string vertLayoutMacros)
        : Material()
        , _vertLayoutMacros(vertLayoutMacros)
    {
    }
    StandardMaterial(const StandardMaterial &src)
        : Material(src)
        , _vertLayoutMacros(src._vertLayoutMacros)
    {
    }
    virtual ~StandardMaterial() {}

protected:
    virtual void Initialize() override
    {
        ShaderManager& shaderMgr = ShaderMgrInstance();
        char* vertMarco = const_cast<char*>(_vertLayoutMacros.c_str());
        char* vertMacros[] = { vertMarco };
        ShaderOption options[] = 
        {
            ShaderOption::OptionData{std::string("Assets/Shaders/standard.vert"), ShaderType::VERTEX_SHADER, 1, vertMacros},
            ShaderOption::OptionData{std::string("Assets/Shaders/standard.frag"), ShaderType::FRAGMENT_SHADER, 1, vertMacros}
        };

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &StandardMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    virtual Material::Ptr SharedClone() override 
    {
        Material::Ptr clone = std::make_shared<StandardMaterial>(*this);
        return clone;
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
            this->ApplyRenderPass();

            vec4 tintColor(1.0f, 1.0f, 1.0f, 1.0f);
            this->SetVecUniformValue<float32, 4>("tintColor", tintColor);

            this->SetValid(true);
        }
    }

protected:
    std::string _vertLayoutMacros;

};

class __TWCOMExport BasicGeomMaterial : public Material
{
public:
    typedef std::shared_ptr<BasicGeomMaterial> Ptr;

    BasicGeomMaterial()
        : Material()
    {
    }
    BasicGeomMaterial(const BasicGeomMaterial &src)
        : Material(src)
    {
    }
    virtual ~BasicGeomMaterial() {}

protected:
    virtual void Initialize() override
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

    virtual Material::Ptr SharedClone() override 
    {
        Material::Ptr clone = std::make_shared<BasicGeomMaterial>(*this);
        return clone;
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
            this->ApplyRenderPass();

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
    }
    SpriteMaterial(const SpriteMaterial &src)
        : Material(src)
    {
    }
    virtual ~SpriteMaterial() {}

protected:
    virtual void Initialize() override
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

    virtual Material::Ptr SharedClone() override 
    {
        Material::Ptr clone = std::make_shared<SpriteMaterial>(*this);
        return clone;
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
            this->ApplyRenderPass();

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
    }
    Sprite1DMaterial(const Sprite1DMaterial &src)
        : Material(src)
    {
    }
    virtual ~Sprite1DMaterial() {}

protected:
    virtual void Initialize() override
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

    virtual Material::Ptr SharedClone() override 
    {
        Material::Ptr clone = std::make_shared<Sprite1DMaterial>(*this);
        return clone;
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
            this->ApplyRenderPass();

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
    }
    LineMaterial(const LineMaterial &src)
        : Material(src)
    {
    }
    virtual ~LineMaterial() {}

protected:
    virtual void Initialize() override
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

    virtual Material::Ptr SharedClone() override 
    {
        Material::Ptr clone = std::make_shared<LineMaterial>(*this);
        return clone;
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
            this->ApplyRenderPass();

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
    }
    InfinitePlaneMaterial(const InfinitePlaneMaterial &src)
        : Material(src)
    {
    }
    virtual ~InfinitePlaneMaterial() {}

protected:
    virtual void Initialize() override
    {
        ShaderManager& shaderMgr = ShaderMgrInstance();
 
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/infinitePlane.vert"), ShaderType::VERTEX_SHADER},
            ShaderOption::OptionData{std::string("Assets/Shaders/infinitePlane.frag"), ShaderType::FRAGMENT_SHADER}};

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &InfinitePlaneMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    virtual Material::Ptr SharedClone() override 
    {
        Material::Ptr clone = std::make_shared<InfinitePlaneMaterial>(*this);
        return clone;
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
            this->ApplyRenderPass();

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
    }
    VolumnQuadMaterial(const VolumnQuadMaterial &src)
        : Material(src)
    {
    }
    virtual ~VolumnQuadMaterial() {}

protected:
    virtual void Initialize() override
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

    virtual Material::Ptr SharedClone() override 
    {
        Material::Ptr clone = std::make_shared<VolumnQuadMaterial>(*this);
        return clone;
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
            this->ApplyRenderPass();

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
    }
    CubeMaterial(const CubeMaterial &src)
        : Material(src)
    {
    }
    virtual ~CubeMaterial() {}

protected:
    virtual void Initialize() override
    {
        ShaderManager& shaderMgr = ShaderMgrInstance();
 
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/cube.vert"), ShaderType::VERTEX_SHADER},
            ShaderOption::OptionData{std::string("Assets/Shaders/cube.frag"), ShaderType::FRAGMENT_SHADER}};

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &CubeMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    virtual Material::Ptr SharedClone() override 
    {
        Material::Ptr clone = std::make_shared<CubeMaterial>(*this);
        return clone;
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
            this->ApplyRenderPass();

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
    }
    SphereMaterial(const SphereMaterial &src)
        : Material(src)
    {
    }
    virtual ~SphereMaterial() {}

protected:
    virtual void Initialize() override
    {
        ShaderManager& shaderMgr = ShaderMgrInstance();
 
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/sphere.vert"), ShaderType::VERTEX_SHADER},
            ShaderOption::OptionData{std::string("Assets/Shaders/sphere.frag"), ShaderType::FRAGMENT_SHADER}};

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &SphereMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    virtual Material::Ptr SharedClone() override 
    {
        Material::Ptr clone = std::make_shared<SphereMaterial>(*this);
        return clone;
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
            this->ApplyRenderPass();

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
    }
    SkyboxMaterial(const SkyboxMaterial &src)
        : Material(src)
    {
    }
    virtual ~SkyboxMaterial() {}

protected:
    virtual void Initialize() override
    {
        ShaderManager& shaderMgr = ShaderMgrInstance();
 
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/skybox.vert"), ShaderType::VERTEX_SHADER},
            ShaderOption::OptionData{std::string("Assets/Shaders/skybox.frag"), ShaderType::FRAGMENT_SHADER}};

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &SkyboxMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    virtual Material::Ptr SharedClone() override 
    {
        Material::Ptr clone = std::make_shared<SkyboxMaterial>(*this);
        return clone;
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
            this->ApplyRenderPass();

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
    }
    ProjectionMappingMaterial(const ProjectionMappingMaterial &src)
        : Material(src)
    {
    }
    virtual ~ProjectionMappingMaterial() {}

protected:
    virtual void Initialize() override
    {
        ShaderManager& shaderMgr = ShaderMgrInstance();
 
        ShaderOption options[] = {
            ShaderOption::OptionData{std::string("Assets/Shaders/projectionTexture.vert"), ShaderType::VERTEX_SHADER},
            ShaderOption::OptionData{std::string("Assets/Shaders/projectionTexture.frag"), ShaderType::FRAGMENT_SHADER}};

        ShaderProgramOption programOption(options, 2);
        programOption.AddSuccessFunc(-1, this, &ProjectionMappingMaterial::OnMaterialSuccess);
        shaderMgr.ReadShadersAsync(&programOption, 2);
    }

    virtual Material::Ptr SharedClone() override 
    {
        Material::Ptr clone = std::make_shared<ProjectionMappingMaterial>(*this);
        return clone;
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
            this->ApplyRenderPass();

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
    }
    ScreenQuadMaterial(const ScreenQuadMaterial &src)
        : Material(src)
    {
    }
    virtual ~ScreenQuadMaterial() {}

protected:
    virtual void Initialize() override
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

    virtual Material::Ptr SharedClone() override 
    {
        Material::Ptr clone = std::make_shared<ScreenQuadMaterial>(*this);
        return clone;
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
            this->ApplyRenderPass();

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
    }
    MSAAResolveMaterial(const MSAAResolveMaterial &src)
        : Material(src)
    {
    }
    virtual ~MSAAResolveMaterial() 
    {
        Console::LogWarning("Deconstruct MSAA resolve material\n");
    }

protected:
    virtual void Initialize() override
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

    virtual Material::Ptr SharedClone() override 
    {
        Material::Ptr clone = std::make_shared<MSAAResolveMaterial>(*this);
        return clone;
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
            this->ApplyRenderPass();

            this->SetValid(true);
        }
    }     
};

} // namespace TwinkleGraphics

#endif