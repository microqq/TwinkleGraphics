// https://senlinzhan.github.io/2017/10/08/gtest/

#include <gtest/gtest.h>
#include <numeric>
#include <vector>

#include "twMaterial.h"

using namespace TwinkleGraphics;

ImageData GetImageData() { ImageData data; data.mip[0].data = new int[10]; return data; }

TEST(MaterialTests, UniformSetting)
{
    // ImageManagerInst imageMgr;
    // ImageReadInfo images_info[] =
    // {
    //     {"Assets/Textures/test.dds"}
    // };
    // Image::Ptr image = imageMgr->ReadImage(images_info[0]);

    // Texture2D::Ptr texture = nullptr;
    // if(image != nullptr)
    // {
    //     texture = std::make_shared<Texture2D>(true);
    //     texture->CreateFromImage(image);
    // }

    // ShaderManagerInst shaderMgr;

    // ShaderReadInfo sprite_shader_info[] = {
    //     {std::string("Assets/Shaders/sprite.vert"), ShaderType::VERTEX_SHADER},
    //     {std::string("Assets/Shaders/sprite.frag"), ShaderType::FRAGMENT_SHADER}};
    // ShaderProgram::Ptr sprite_shader = shaderMgr->ReadShaders(sprite_shader_info, 2);

    // RenderPass::Ptr pass0 = std::make_shared<RenderPass>(sprite_shader);
    // Material::Ptr material = std::make_shared<Material>();

    // material->AddRenderPass(pass0);
    // material->SetMainTexture(texture);

    // material->SetSimpleUniformValue<bool, 1>("light_on", true);
<<<<<<< HEAD
    // vec4 tint_color;
    // material->SetVecUniformValue<float32, 4>("tint_color", tint_color);
=======
    // vec4 tintColor;
    // material->SetVecUniformValue<float32, 4>("tintColor", tintColor);
>>>>>>> twinkle_shading

    SimpleUniform<float32, 1> simpleuniform("test");
    simpleuniform.u0 = 5.0f;

    SimpleUniform<float32, 1> simpleuniform2(simpleuniform);
    ASSERT_EQ(simpleuniform2.u0, 5.0f);

    VecUniform<float32, 2> vecuniform("vec_test");
    vec2 vec(3.0f, 2.0f);
    vecuniform.Set(vec);

    VecUniform<float32, 2> vecuniform2(vecuniform);
    ASSERT_EQ(vecuniform2.vector.x, 3.0f);
    ASSERT_EQ(vecuniform2.vector.y, 2.0f);


    // material
    RenderPass::Ptr pass0 = std::make_shared<RenderPass>();
    Material::Ptr material = std::make_shared<Material>();
    material->AddRenderPass(pass0);

<<<<<<< HEAD
    vec4 tint_color(2.0f, 3.0f, 4.0f, 5.0f);
    material->SetVecUniformValue<float32, 4>("tint_color", tint_color);

    //material2
    Material::Ptr material2 = std::make_shared<Material>(*(material.get()));
    const VecUniform<float32, 4> *cast_vec4 = dynamic_cast<const VecUniform<float32, 4>* >(material2->GetUniform("tint_color"));
=======
    vec4 tintColor(2.0f, 3.0f, 4.0f, 5.0f);
    material->SetVecUniformValue<float32, 4>("tintColor", tintColor);

    //material2
    Material::Ptr material2 = std::make_shared<Material>(*(material.get()));
    const VecUniform<float32, 4> *cast_vec4 = dynamic_cast<const VecUniform<float32, 4>* >(material2->GetUniform("tintColor"));
>>>>>>> twinkle_shading
    ASSERT_EQ(cast_vec4->vector, vec4(2.0f, 3.0f, 4.0f, 5.0f));

    ImageData data = GetImageData();

    ImageData imagedatacopy = GetImageData();
    ASSERT_EQ(imagedatacopy.mip[0].data != nullptr, true);

    // std::vector<int> vec{1, 2, 3, 4, 5};
    // int sum = std::accumulate(vec.begin(), vec.end(), 0);
    // EXPECT_EQ(sum, 15);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}