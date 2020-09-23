
#ifndef TW_MODELREADER_H
#define TW_MODELREADER_H

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "twResource.h"
#include "twModel.h"
#include "twMaterialInstance.h"

namespace TwinkleGraphics
{
    class __TWCOMExport ModelReader final : public ResourceReader
        , public Reference<ModelReader>
        , public INonCopyable
    {
    public:
        typedef std::shared_ptr<ModelReader> Ptr;

        ModelReader();
        ModelReader(ReaderOption* option);
        virtual ~ModelReader();

        ReadResult<Model> Read(const char *filename);
        ReadResult<Model> ReadAsync(std::string filename);

        void SetOption(ReaderOption* option) 
        {
            if(option == nullptr)
            {
                return;                
            }

            if (_option != nullptr)
            {
                SAFE_DEL(_option);
            }            
            _option = new ReaderOption(*option);
        }

        DECLARE_READERID;

    private:
        Geometry::Ptr ProcessNode(aiNode *node, const aiScene *scene, std::string dir, Model::Ptr model, Material::Ptr* vecMats);
        SubMesh::Ptr ProcessMesh(aiMesh *mesh, Mesh::Ptr tMesh, int32 offset, const aiScene *scene);
        Material::Ptr ProcessMaterial(aiMesh *mesh, const aiScene *scene, aiMaterial *mat, std::string dir, VertexLayoutFlag layoutFalg);
        std::vector<Texture::Ptr> LoadTextures(aiMaterial *mat, aiTextureType type, std::string dir);
        void SetMaterialTextures(std::vector<Texture::Ptr>& textures
            , StandardMaterial::Ptr material
            , std::string texNamePrefix
            , std::string macroPrefix);
    };

} // namespace TwinkleGraphics

#endif