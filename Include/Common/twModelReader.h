
#ifndef TW_MODELREADER_H
#define TW_MODELREADER_H

#include "assimp/Importer.hpp"
#include "assimp/postprocess.h"
#include "assimp/scene.h"


#include "twMaterialInstance.h"
#include "twModel.h"
#include "twResource.h"


namespace TwinkleGraphics {
class __TWCOMExport ModelReader final : public ResourceReader,
                                        public Reference<ModelReader>,
                                        public INonCopyable {
public:
  typedef std::shared_ptr<ModelReader> Ptr;

  ModelReader();
  ModelReader(ReaderOption *option);
  virtual ~ModelReader();

  ReadResult<Model> Read(const char *filename);
  ReadResult<Model> ReadAsync(std::string filename);

  void SetOption(ReaderOption *option) {
    if (option == nullptr) {
      return;
    }

    if (_option != nullptr) {
      SAFE_DEL(_option);
    }
    _option = new ReaderOption(*option);
  }

  DECLARE_READERID;

private:
  GeometryPtr ProcessNode(aiNode *node, const aiScene *scene, std::string dir,
                          ModelPtr model, MaterialPtr *vecMats);
  SubMeshPtr ProcessMesh(aiMesh *mesh, MeshPtr tMesh, int32 offset,
                         const aiScene *scene);
  MaterialPtr ProcessMaterial(aiMesh *mesh, const aiScene *scene,
                              aiMaterial *mat, std::string dir,
                              VertexLayoutFlag layoutFalg);
  std::vector<TexturePtr> LoadTextures(aiMaterial *mat, aiTextureType type,
                                       std::string dir);
  void SetMaterialTextures(std::vector<TexturePtr> &textures,
                           StandardMaterial::Ptr material,
                           std::string texNamePrefix, std::string macroPrefix);
};

} // namespace TwinkleGraphics

#endif