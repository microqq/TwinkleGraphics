// https://senlinzhan.github.io/2017/10/08/gtest/

#include <gtest/gtest.h>
#include <numeric>
#include <vector>

#include "twMaterial.h"

using namespace TwinkleGraphics;

ImageData GetImageData() { ImageData data; data.mip[0].data = new int[10]; return data; }

TEST(MaterialTests, UniformSetting)
{
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
    RenderPassPtr pass0 = std::make_shared<RenderPass>();
    MaterialPtr material = std::make_shared<Material>();
    material->AddRenderPass(pass0);

    vec4 tintColor(2.0f, 3.0f, 4.0f, 5.0f);
    material->SetVecUniformValue<float32, 4>("tintColor", tintColor);

    //material2
    MaterialPtr material2 = std::make_shared<Material>(*(material.get()));
    const VecUniform<float32, 4> *cast_vec4 = dynamic_cast<const VecUniform<float32, 4>* >(material2->GetUniform("tintColor"));
    ASSERT_EQ(cast_vec4->vector, vec4(2.0f, 3.0f, 4.0f, 5.0f));

    ImageData imagedatacopy = GetImageData();
    ASSERT_EQ(imagedatacopy.mip != nullptr && imagedatacopy.mip[0].data != nullptr, false);

    // std::vector<int> vec{1, 2, 3, 4, 5};
    // int sum = std::accumulate(vec.begin(), vec.end(), 0);
    // EXPECT_EQ(sum, 15);
}

