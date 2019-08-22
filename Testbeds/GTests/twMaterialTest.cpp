// https://senlinzhan.github.io/2017/10/08/gtest/

#include <gtest/gtest.h>
#include <numeric>
#include <vector>

#include "twMaterial.h"

using namespace TwinkleGraphics;

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
    //     texture->SetImage(image);
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
    // vec4 tint_color;
    // material->SetVecUniformValue<float32, 4>("tint_color", tint_color);

    std::vector<int> vec{1, 2, 3, 4, 5};
    int sum = std::accumulate(vec.begin(), vec.end(), 0);
    EXPECT_EQ(sum, 15);
}

int main(int argc, char *argv[])
{
    ::testing::InitGoogleTest(&argc, argv);
    return RUN_ALL_TESTS();
}