
#ifndef TW_MATERIAL_H
#define TW_MATERIAL_H

#include <vector>
#include <map>

#include "twCommon.h"
#include "twShader.h"
#include "twRenderContext.h"
#include "twTexture.h"

namespace TwinkleGraphics
{

/**
 * Material Construct Steps:
 * 1、Material:Construct Material
 * 2、RenderPass:Construct RenderPass
 * 3、RenderPass:Set ShaderProgram
 * 4、RenderPass:Set RenderState
 * 5、Material:Set RenderPass
 * 6、Material:Set Texture
 * 7、Material:Set Uniform
 * 8、Material:Set RenderState
*/


class RenderPass;
class Material;

/**
 * @brief 
 * 
 */
class RenderPass : public Object
{
public:
    typedef std::shared_ptr<RenderPass> Ptr;

    RenderPass(ShaderProgram::Ptr shader = nullptr);
    RenderPass(const RenderPass& copy);
    virtual ~RenderPass();

    void SetCullMode(CullMode cull) {}
    void SetPolygonMode(PolygonMode polygonmode) {}

    void SetShader(ShaderProgram::Ptr shader) { _shader = shader; }
    void SetEnable(bool enable) { _enable = enable; }

    inline bool Enabled() { return _enable; }
    inline const RenderState& GetRenderState() { return _state; }
    inline const ShaderProgram::Ptr& GetShader() { return _shader; }
    inline const std::map<std::string, TextureSlot>& GetTextureSlots() { return _slots; }
    inline const std::map<std::string, UniformLocation>& GetUniformLocations() { return _uniformlocations; }

#ifdef TEMPORARY_USE
    void Apply();
#endif

private:
    void SetMaintexture(Texture::Ptr maintex) { SetTexture("main_tex", maintex); }
    void SetTexture(const char* name, Texture::Ptr tex);
    void SetUniform(const char* name, Uniform* uniform);

private:
    std::map<std::string, TextureSlot> _slots;
    std::map<std::string, UniformLocation> _uniformlocations;
    RenderState _state;
    ShaderProgram::Ptr _shader;

    bool _enable;

    friend class Material;
};


/**
 * @brief 
 * 
 */
class Material : public Object
{
public:
    typedef std::shared_ptr<Material> Ptr;

    Material();
    Material(const Material& copy);
    virtual ~Material();

    void SetCullMode(CullMode cull) {}
    void SetPolygonMode(PolygonMode polygonmode) {}

    /**
     * @brief Set the Render Pass object
     * 
     * @param index 
     * @param pass 
     */
    void SetRenderPass(int32 index, const RenderPass::Ptr pass) { if(index < _passes.size()) _passes[index] = pass; }

    /**
     * @brief 
     * 
     * @param pass 
     */
    void AddRenderPass(const RenderPass::Ptr pass) { _passes.push_back(pass); }

    RenderPass::Ptr GetRenderPass(int32 index) { if(index >= 0 && index < _passes.size()) return _passes[index]; return nullptr; }

    /**
     * @brief Set the Main Texture object
     * 
     * @param maintex 
     */
    void SetMainTexture(Texture::Ptr maintex);

    /**
     * @brief Set the Texture object
     * 
     * @param name 
     * @param tex 
     */
    void SetTexture(const char* name, Texture::Ptr tex);

    Texture::Ptr GetMainTexture() { return GetTexture("main_tex"); }
    Texture::Ptr GetTexture(const char* name);

    /**
     * @brief Set the Texture Tiling object
     * 
     * @param name 
     * @param tiling 
     */
    void SetTextureTiling(const char* name, vec2& tiling);

    /**
     * @brief Set the Texture Offset object
     * 
     * @param name 
     * @param offset 
     */
    void SetTextureOffset(const char* name,vec2& offset);

    /**
     * @brief Set the Simple Uniform Value object
     * 
     * @tparam T: bool, float, double, int, uint,
     * @tparam N: 1, 2, 3, 4
     * @tparam Args
     * @param name 
     * @param args: T t1, T t2, ...
     */
    template<class T, uint32 N, class ...Args>
    void SetSimpleUniformValue(const char* name, Args&&...args)
    {
        SimpleUniform<T, N> *uniform = nullptr;

        std::map<std::string, Uniform *>::iterator it = _uniforms.find(name);
        if (it == _uniforms.end())
        {
            uniform = new SimpleUniform<T, N>(name);
            AddUniform(name, uniform);
            SetPassesUniform(name, uniform);
        }
        else
        {
            uniform = dynamic_cast<SimpleUniform<T, N> *>(it->second);
        }

        uniform->Set(std::forward<Args>(args)...);
    }


    /**
     * @brief Set the Vec Uniform Value object
     * 
     * @tparam T: bool, float, double, int, uint,
     * @tparam N: 2, 3, 4 
     * @param name 
     * @param value 
     */
    template <class T, uint32 N>
    void SetVecUniformValue(const char *name, vec<N, T, defaultp>& value)
    {
        VecUniform<T, N> *uniform = nullptr;

        std::map<std::string, Uniform *>::iterator it = _uniforms.find(name);
        if (it == _uniforms.end())
        {
            uniform = new VecUniform<T, N>(name);
            AddUniform(name, uniform);
            SetPassesUniform(name, uniform);
        }
        else
        {
            uniform = dynamic_cast<VecUniform<T, N> *>(it->second);
        }

        uniform->Set(value);
    }

    /**
     * @brief Set the Matrix Uniform Value object
     * 
     * @tparam T: bool, float, double, int, uint, 
     * @tparam Row: 2, 3, 4 
     * @tparam Column: 2, 3, 4 
     * @param name 
     * @param value 
     */
    template <class T, uint32 Row, uint32 Column>
    void SetMatrixUniformValue(const char *name, mat<Row, Column, T, defaultp>& value, bool transpose = false)
    {
        MatUniform<T, Row, Column> *uniform = nullptr;

        std::map<std::string, Uniform *>::iterator it = _uniforms.find(name);
        if (it == _uniforms.end())
        {
            uniform = new MatUniform<T, Row, Column>(name);
            AddUniform(name, uniform);
            SetPassesUniform(name, uniform);
        }
        else
        {
            uniform = dynamic_cast<MatUniform<T, Row, Column> *>(it->second);
        }

        uniform->Set(value, transpose);
    }

    /**
     * @brief Get the Render State object
     * 
     * @return const RenderState& 
     */
    const RenderState &GetRenderState() { return _state; }

    /**
     * @brief Get the Uniform object
     * 
     * @param name 
     * @return const Uniform* 
     */
    const Uniform* GetUniform(const char* name);


private:
    /**
     * @brief 
     * 
     * @param name 
     * @param uniform 
     */
    void AddUniform(const char* name, Uniform* uniform);

    /**
     * @brief 
     * 
     * @param name 
     */
    void RemoveUniform(const char* name);

    /**
     * @brief Set the Passes Uniform object
     * 
     * @param name 
     * @param uniform 
     */
    void SetPassesUniform(const char* name, Uniform* uniform);


private:
    std::vector<RenderPass::Ptr> _passes;
    std::map<std::string, Uniform*> _uniforms;
    std::map<std::string, Texture::Ptr> _textures;

    RenderState _state;
};


} // namespace TwinkleGraphics


#endif