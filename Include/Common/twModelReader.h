
#ifndef TW_MODELREADER_H
#define TW_MODELREADER_H

#include "assimp/Importer.hpp"
#include "assimp/scene.h"
#include "assimp/postprocess.h"

#include "twResource.h"
#include "twModel.h"

namespace TwinkleGraphics
{
    struct ModelSource;
    class Model;
    class ModelManager;
    typedef Singleton<ModelManager> ModelManagerInst;

    struct ModelSource : public SourceHandle
    {
        typedef std::shared_ptr<ModelSource> Ptr;
        typedef std::weak_ptr<ModelSource> WeakPtr;

        std::string filename;
        Assimp::Importer* importer = nullptr;
    };

    class __TWExport ModelReader final : public ResourceReader
    {
    public:
        ModelReader();
        virtual ~ModelReader();

        template <typename T>
        ReadResult<T> Read(const char *filename, ReaderOption *option);
        ReadResult<Model> ReadAsync(const char *filename, ReaderOption *option);

        Geometry::Ptr ProcessNode(aiNode *node, const aiScene *scene, std::string dir, Model::Ptr model, Material::Ptr vecMats[]);
        SubMesh::Ptr ProcessMesh(aiMesh *mesh, Mesh::Ptr tMesh, int32 offset, const aiScene *scene);
        Material::Ptr ProcessMaterial(aiMesh *mesh, const aiScene *scene, aiMaterial *mat, std::string dir, VertexLayoutFlag layoutFalg);
        std::vector<Texture::Ptr> LoadTextures(aiMaterial *mat, aiTextureType type, std::string dir);
        void SetMaterialTextures(std::vector<Texture::Ptr>& textures, Material::Ptr material, std::string texNamePrefix);

        DECLARE_READERID;
    };

    class __TWExport ModelManager
    {
    public:
        ModelManager() {}
        ~ModelManager() {}

        Model::Ptr ReadModel(const char *filename);
        auto ReadModelAsync(const char* filename, ShaderOption* option)
            -> std::future<ReadResult<Model>>;

    private:
    };
} // namespace TwinkleGraphics

#endif