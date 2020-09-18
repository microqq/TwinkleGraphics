
#include <map>

#include "twModelReader.h"
#include "twMaterialInstance.h"
#include "twConsoleLog.h"
#include "twResourceManager.h"
#include "twImageManager.h"

namespace TwinkleGraphics
{
    ModelReader::ModelReader()
        : ResourceReader()
    {
        // INITIALISE_READERID
    }

    ModelReader::ModelReader(ReaderOption *option)
        : ResourceReader()
    {
        if(option != nullptr)
        {
            if(_option != nullptr)
            {
                SAFE_DEL(_option); 
            }
			_option = new ReaderOption(*option);
        }
    }

    ModelReader::~ModelReader()
    {
    }
    DEFINE_READERID(ModelReader)

    /**
     * 
     */
    ReadResult<Model> ModelReader::Read(const char *filename)
    {
        // https://learnopengl.com/
        Assimp::Importer importer;
        const aiScene* scene = importer.ReadFile(filename, aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_CalcTangentSpace);
        // check for errors
        if(!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) // if is Not Zero
        {
            Console::LogError("ERROR::ASSIMP:: ", importer.GetErrorString(), "\n");
            return ReadResult<Model>(ReadResult<Model>::Status::FAILED);
        }

        Model::Ptr model = std::make_shared<Model>();
        std::string path(filename);
        // retrieve the directory path of the filepath
        std::string directory = path.substr(0, path.find_last_of('\\'));

        Material::Ptr* vectorMaterials = new Material::Ptr[scene->mNumMaterials]{nullptr};
        // Material::Ptr vectorMaterials[30] = {nullptr};

        // process ASSIMP's root node recursively
        Geometry::Ptr rootGeom = ProcessNode(scene->mRootNode, scene, directory, model, vectorMaterials);
        model->SetRootGeometry(rootGeom);

        SAFE_DEL_ARR(vectorMaterials);

        return ReadResult<Model>(shared_from_this(), model, ReadResult<Model>::Status::SUCCESS);
    }

    ReadResult<Model> ModelReader::ReadAsync(std::string filename)
    {
        _asynchronize = true;
        return Read(filename.c_str());
    }

    /**
    *   https://learnopengl.com/
    *   processes a node in a recursive fashion. Processes each individual mesh located at the node and repeats this process on its children nodes (if any).
    */
    Geometry::Ptr ModelReader::ProcessNode(aiNode *node, const aiScene *scene, std::string dir, Model::Ptr model, Material::Ptr* vecMats)
    {
        int32 verticeNum = 0;
        int32 meshFlag = (int32)(MeshDataFlag::DEFAULT);
        // process each mesh located at the current node
        for (unsigned int i = 0; i < node->mNumMeshes; i++)
        {
            // the node object only contains indices to index the actual objects in the scene.
            // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
            aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
            verticeNum += mesh->mNumVertices;

            if(mesh->mColors[0] != nullptr)
            {
                meshFlag |= MeshDataFlag::HAS_COLOR;
            }
            if(mesh->mNormals != nullptr)
            {
                meshFlag |= MeshDataFlag::HAS_NORMAL;
            }
            if(mesh->mTangents != nullptr)
            {
                meshFlag |= MeshDataFlag::HAS_BINORMAL;
            }
            MeshDataFlag texcoordFlags[AI_MAX_NUMBER_OF_TEXTURECOORDS] = 
            {
                MeshDataFlag::HAS_UV,
                MeshDataFlag::HAS_UV1,
                MeshDataFlag::HAS_UV2,
                MeshDataFlag::HAS_UV3,
                MeshDataFlag::HAS_UV4,
                MeshDataFlag::HAS_UV5,
                MeshDataFlag::HAS_UV6,
                MeshDataFlag::HAS_UV7
            };
            for(unsigned int k = 0; k < AI_MAX_NUMBER_OF_TEXTURECOORDS; k++)
            {
                if(mesh->mTextureCoords[k] != nullptr)
                {
                    meshFlag |= texcoordFlags[k];
                }
            }
        }

        Geometry::Ptr geom = std::make_shared<Geometry>();
        Transform::Ptr parentTransform = geom->GetTransform();
        model->AddGeometry(geom);

        if (verticeNum > 0)
        {
            MeshRenderer::Ptr renderer = std::make_shared<MeshRenderer>();
            geom->SetMeshRenderer(renderer);

            Mesh::Ptr tMesh = std::make_shared<Mesh>();
            tMesh->Initialize(verticeNum, MeshDataFlag(meshFlag));

            verticeNum = 0;
            for (unsigned int i = 0; i < node->mNumMeshes; i++)
            {
                // the node object only contains indices to index the actual objects in the scene.
                // the scene contains all the data, node is just to keep stuff organized (like relations between nodes).
                aiMesh *mesh = scene->mMeshes[node->mMeshes[i]];
                SubMesh::Ptr subMesh = ProcessMesh(mesh, tMesh, verticeNum, scene);
                tMesh->AddSubMesh(subMesh);

                if(vecMats[mesh->mMaterialIndex] != nullptr)
                {
                    int32 matIndex = renderer->GetMaterialIndex(vecMats[mesh->mMaterialIndex]);
                    if(matIndex == -1)
                    {
                        subMesh->SetMaterialIndex(renderer->GetMaterialCount());
                        renderer->AddMaterial(vecMats[mesh->mMaterialIndex]);
                        vecMats[mesh->mMaterialIndex] = renderer->GetMaterial(renderer->GetMaterialCount() - 1);
                    }
                    else
                    {
                        subMesh->SetMaterialIndex(renderer->GetMaterialIndex(vecMats[mesh->mMaterialIndex]));
                    }
                }
                else
                {
                    Material::Ptr material = ProcessMaterial(mesh, scene, scene->mMaterials[mesh->mMaterialIndex], dir, (VertexLayoutFlag)meshFlag);

                    subMesh->SetMaterialIndex(renderer->GetMaterialCount());
                    renderer->AddMaterial(material);
                    vecMats[mesh->mMaterialIndex] = renderer->GetMaterial(renderer->GetMaterialCount() - 1);
                }

                verticeNum += mesh->mNumVertices;
            }

            geom->SetMeshExternal(tMesh);
            renderer->SetMesh(tMesh);
        }

        // after we've processed all of the meshes (if any) we then recursively process each of the children nodes
        for (unsigned int i = 0; i < node->mNumChildren; i++)
        {
            Geometry::Ptr childGeom = ProcessNode(node->mChildren[i], scene, dir, model, vecMats);
            Transform::Ptr childTransform = childGeom->GetTransform();
            childTransform->SetParent(parentTransform);
        }

        return geom;
    }

    /**
    *   https://learnopengl.com/
    */
    SubMesh::Ptr ModelReader::ProcessMesh(aiMesh *mesh, Mesh::Ptr tMesh, int32 offset, const aiScene *scene)
    {
        SubMesh::Ptr subMesh = std::make_shared<SubMesh>();

        int32 indiceNum = 0;
        // now wak through each of the mesh's faces (a face is a mesh its triangle) and retrieve the corresponding vertex indices.
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            indiceNum += face.mNumIndices;
        }
        subMesh->Initialize(indiceNum);
        uint32* indices = subMesh->GetIndice();

        int indiceIndex = 0;
        for(unsigned int i = 0; i < mesh->mNumFaces; i++)
        {
            aiFace face = mesh->mFaces[i];
            // retrieve all indices of the face and store them in the indices vector
            for(unsigned int j = 0; j < face.mNumIndices; j++)
            {
                indices[indiceIndex++] = face.mIndices[j] + offset;
            }
        }

        glm::vec3* verticePos = tMesh->GetVerticePos();
        glm::vec3* verticeNormal = tMesh->GetVerticeNormal();
        glm::vec3* verticeBinormal = tMesh->GetVerticeBiNormal();
        glm::vec4* verticeColor = tMesh->GetVerticeColor();
        glm::vec4* verticeUV = tMesh->GetVerticeUV();
        glm::vec4* verticeUV1 = tMesh->GetVerticeUV1();
        glm::vec4* verticeUV2 = tMesh->GetVerticeUV2();
        glm::vec4* verticeUV3 = tMesh->GetVerticeUV3();
        glm::vec4* verticeUV4 = tMesh->GetVerticeUV4();
        glm::vec4* verticeUV5 = tMesh->GetVerticeUV5();
        glm::vec4* verticeUV6 = tMesh->GetVerticeUV6();
        glm::vec4* verticeUV7 = tMesh->GetVerticeUV7();

        verticePos += offset;
        if(verticeNormal != nullptr)
        {
            verticeNormal += offset;
        }
        if(verticeBinormal != nullptr)
        {
            verticeBinormal += offset;
        }
        if(verticeColor != nullptr)
        {
            verticeColor += offset;
        }
        // texture coordinates
        glm::vec4 *uvs[AI_MAX_NUMBER_OF_TEXTURECOORDS] = {
            verticeUV,
            verticeUV1,
            verticeUV2,
            verticeUV3,
            verticeUV4,
            verticeUV5,
            verticeUV6,
            verticeUV7
        };

        for(int32 s = 0; s < AI_MAX_NUMBER_OF_TEXTURECOORDS; s++)
        {
            if (uvs[s] != nullptr)
            {
                uvs[s] += offset;
            }
        }

        // walk through each of the mesh's vertices
        for(unsigned int i = 0; i < mesh->mNumVertices; i++)
        {
            glm::vec3 vector; // we declare a placeholder vector since assimp uses its own vector class that doesn't directly convert to glm's vec3 class so we transfer the data to this placeholder glm::vec3 first.
            // positions
            vector.x = mesh->mVertices[i].x;
            vector.y = mesh->mVertices[i].y;
            vector.z = mesh->mVertices[i].z;
            verticePos[i] = vector;

            // normals
            if(mesh->mNormals != nullptr)
            {
                vector.x = mesh->mNormals[i].x;
                vector.y = mesh->mNormals[i].y;
                vector.z = mesh->mNormals[i].z;
                verticeNormal[i] = vector;
            }


            for(unsigned int j = 0; j < AI_MAX_NUMBER_OF_TEXTURECOORDS; j++)
            {
                if(uvs[j] != nullptr)
                {
                    if(mesh->mTextureCoords[j])
                    {
                        glm::vec4 vec;
                        // a vertex can contain up to 8 different texture coordinates. We thus make the assumption that we won't 
                        // use models where a vertex can have multiple texture coordinates so we always take the first set (0).
                        vec.x = mesh->mTextureCoords[j][i].x; 
                        vec.y = 1.0f - mesh->mTextureCoords[j][i].y;
                        uvs[j][i] = vec;
                    }
                    else
                    {
                        uvs[j][i] = glm::vec4(0.0f, 0.0f, 0.0f, 0.0f);             
                    }
                }
            }

            // AI_MAX_NUMBER_OF_COLOR_SETS
            if(mesh->mColors[0] != nullptr)
            {
                glm::vec4 vecColor;
                vecColor.r = mesh->mColors[0][i].r;
                vecColor.g = mesh->mColors[0][i].g;
                vecColor.b = mesh->mColors[0][i].b;
                vecColor.a = mesh->mColors[0][i].a;
                verticeColor[i] = vecColor;
            }

            // tangent
            if(mesh->mTangents != nullptr)
            {
                vector.x = mesh->mTangents[i].x;
                vector.y = mesh->mTangents[i].y;
                vector.z = mesh->mTangents[i].z;
                verticeBinormal[i] = vector;
            }
        }

        verticePos = nullptr;
        verticeNormal = nullptr;
        verticeBinormal = nullptr;
        verticeColor = nullptr;
        verticeUV = nullptr;
        verticeUV1 = nullptr;
        verticeUV2 = nullptr;
        verticeUV3 = nullptr;
        verticeUV4 = nullptr;
        verticeUV5 = nullptr;
        verticeUV6 = nullptr;
        verticeUV7 = nullptr;

        return subMesh;
    }

    /**
    *   https://learnopengl.com/
    */
    Material::Ptr ModelReader::ProcessMaterial(aiMesh *mesh, const aiScene *scene, aiMaterial *mat, std::string dir, VertexLayoutFlag layoutFlag)
    {
        std::string vertLayoutMacros("");
        int32 operand = 1;
        int32 index = 0;
        int32 numVerteLayoutMacros = 0;
        while (index <= 10)
        {
            /* code */
            int32 logicAnd = operand & (int32)layoutFlag;
            if(logicAnd > 0)
            {
                vertLayoutMacros += VertexLayoutDefines[index];
                numVerteLayoutMacros++;
            }
            operand <<= 1;
            index++;
        }

        StandardMaterial::Ptr material = std::make_shared<StandardMaterial>(vertLayoutMacros);

        // process materials
        aiMaterial* aiMat = scene->mMaterials[mesh->mMaterialIndex];    
        // we assume a convention for sampler names in the shaders. Each diffuse texture should be named
        // as 'texture_diffuseN' where N is a sequential number ranging from 1 to MAX_SAMPLER_NUMBER. 
        // Same applies to other texture as the following list summarizes:
        // diffuse: texture_diffuseN
        // specular: texture_specularN
        // normal: texture_normalN

        // 1. diffuse maps
        std::vector<Texture::Ptr> diffuseMaps = LoadTextures(aiMat, aiTextureType_DIFFUSE, dir);
        SetMaterialTextures(diffuseMaps, material, "diffuseTex");

        // 2. specular maps
        std::vector<Texture::Ptr> specularMaps = LoadTextures(aiMat, aiTextureType_SPECULAR, dir);
        SetMaterialTextures(specularMaps, material, "specularTex");

        // 3. normal maps
        std::vector<Texture::Ptr> normalMaps = LoadTextures(aiMat, aiTextureType_NORMALS, dir);
        SetMaterialTextures(normalMaps, material, "normalTex");
        
        // 4. height maps
        std::vector<Texture::Ptr> heightMaps = LoadTextures(aiMat, aiTextureType_HEIGHT, dir);
        SetMaterialTextures(heightMaps, material, "heightTex");

        // 5. amibient maps
        std::vector<Texture::Ptr> ambientMaps = LoadTextures(aiMat, aiTextureType_AMBIENT, dir);
        SetMaterialTextures(ambientMaps, material, "ambientTex");

        // 6. emissive maps
        std::vector<Texture::Ptr> emissiveMaps = LoadTextures(aiMat, aiTextureType_EMISSIVE, dir);
        SetMaterialTextures(emissiveMaps, material, "emissiveTex");

        // 7. shininess maps
        std::vector<Texture::Ptr> shininessMaps = LoadTextures(aiMat, aiTextureType_SHININESS, dir);
        SetMaterialTextures(shininessMaps, material, "shininessTex");

        // 8. opacity maps
        std::vector<Texture::Ptr> opacityMaps = LoadTextures(aiMat, aiTextureType_OPACITY, dir);
        SetMaterialTextures(opacityMaps, material, "opacityTex");

        // 9. displacement maps
        std::vector<Texture::Ptr> displacementMaps = LoadTextures(aiMat, aiTextureType_DISPLACEMENT, dir);
        SetMaterialTextures(displacementMaps, material, "displacementTex");

        // 10. lightmap maps
        std::vector<Texture::Ptr> lightmapMaps = LoadTextures(aiMat, aiTextureType_LIGHTMAP, dir);
        SetMaterialTextures(lightmapMaps, material, "lightmapTex");

        // 11. reflection maps
        std::vector<Texture::Ptr> reflectionMaps = LoadTextures(aiMat, aiTextureType_REFLECTION, dir);
        SetMaterialTextures(reflectionMaps, material, "reflectionTex");

        return material;
    }

    void ModelReader::SetMaterialTextures(std::vector<Texture::Ptr>& textures, Material::Ptr material, std::string texNamePrefix)
    {
        int32 number = 0;
        for(auto texture : textures)
        {
             std::string texVarName = texNamePrefix + std::to_string(number++);
             material->SetTexture(texVarName.c_str(), texture);
        }
    }

    // checks all material textures of a given type and loads the textures if they're not loaded yet.
    // the required info is returned as a Texture struct.
    std::vector<Texture::Ptr> ModelReader::LoadTextures(aiMaterial *mat, aiTextureType type, std::string dir)
    {
        std::vector<Texture::Ptr> textures;
        for(unsigned int i = 0; i < mat->GetTextureCount(type); i++)
        {
            aiString str;
            mat->GetTexture(type, i, &str);
            
            // int32 a = 5;
            // if(std::string(str.C_Str()).find("sponza_arch_diff") && type == aiTextureType_DIFFUSE)
            // {
            //     assert(a != 5);
            //     std::cout << str.C_Str() << std::endl;
            // }

            Texture2D::Ptr texture = std::make_shared<Texture2D>(true, true);
            ImageManager& imageMgr = ImageMgrInstance();
            std::string imgFilename{dir + "/" + std::string(str.C_Str())};

            ImageOption option;
            ReadSuccessCallbackFuncPtr funcPtr = std::make_shared<ReadSuccessCallbackFunc>(
                [texture](Object::Ptr obj) {
                    Image::Ptr image = std::dynamic_pointer_cast<Image>(obj);
                    if (image != nullptr)
                    {
                        ImageData *imageData = image->GetImageSourcePtr();
                        float fwh = imageData->mip[0].width > imageData->mip[0].height ? imageData->mip[0].height : imageData->mip[0].width;
                        int32 level = 0;
                        while (fwh >= 1.0f)
                        {
                            fwh /= 2.0f;
                            level++;
                        }
                        if (level > 10)
                        {
                            level = 10;
                        }

                        imageData->mipLevels = level;
                        texture->CreateFromImage(image);

                        texture->SetWrap<WrapParam::WRAP_S>(WrapMode::REPEAT);
                        texture->SetWrap<WrapParam::WRAP_T>(WrapMode::REPEAT);
                        texture->SetFilter<FilterParam::MIN_FILTER>(FilterMode::LINEAR_MIPMAP_LINEAR);
                        texture->SetFilter<FilterParam::MAG_FILTER>(FilterMode::LINEAR);
                        texture->SetMipMapBaseLevel(MipMapBaseLevelParam::BESE_LEVEL, 0);
                        texture->SetMipMapMaxLevel(MipMapMaxLevelParam::MAX_LEVEL, level - 1);
                    }
                });
            option.AddSuccessFunc(-1, funcPtr);
            imageMgr.ReadImageAsync(imgFilename.c_str(), &option);
            // Image::Ptr image = imageMgr.ReadImage(imgFilename.c_str(), new ImageOption);
            textures.push_back(texture);
        }
        return textures;
    }

} // namespace TwinkleGraphics
