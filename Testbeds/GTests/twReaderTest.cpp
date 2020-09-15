
#include <functional>
#include <string>
#include <gtest/gtest.h>

#include "twConsoleLog.h"
#include "twResource.h"
#include "twShader.h"
#include "twMainWindow.h"
#include "twResourceManager.h"
#include "twTextureManager.h"

using namespace TwinkleGraphics;
using namespace std::chrono_literals;

ShaderOption lineVertOption_(ShaderOption::OptionData{ "Assets/Shaders/line.vert", ShaderType::VERTEX_SHADER });
ShaderOption lineFragOption_(ShaderOption::OptionData{ "Assets/Shaders/line.frag", ShaderType::FRAGMENT_SHADER });

TEST(ResourceReaderTests, Read)
{
    ShaderManager& shaderMgr = ShaderMgrInstance();
    shaderMgr.ReadShader("Assets/Shaders/line.vert", &lineVertOption_);
};

TEST(ResourceReaderTests, ReadAsync)
{
    GLFWMainWindow MainWindow_(1024, 768);

    lineVertOption_.SetStoreHint(CacheStoreHint::TIMELIMITED, 20.0f);
    lineFragOption_.SetStoreHint(CacheStoreHint::TIMELIMITED, 10.0f);

    ShaderManager& shaderMgr = ShaderMgrInstance();
    shaderMgr.ReadShaderAsync("Assets/Shaders/line.vert", &lineVertOption_);
    shaderMgr.ReadShaderAsync("Assets/Shaders/line.vert", &lineVertOption_);
    shaderMgr.ReadShaderAsync("Assets/Shaders/line.vert", &lineVertOption_);

    shaderMgr.ReadShaderAsync("Assets/Shaders/line.frag", &lineFragOption_);
    shaderMgr.ReadShaderAsync("Assets/Shaders/line.frag", &lineFragOption_);
    shaderMgr.ReadShaderAsync("Assets/Shaders/line.frag", &lineFragOption_);

    ShaderOption options[] = {
        ShaderOption::OptionData{std::string("Assets/Shaders/basicGeometry.vert"), ShaderType::VERTEX_SHADER},
        ShaderOption::OptionData{std::string("Assets/Shaders/basicGeometry.frag"), ShaderType::FRAGMENT_SHADER}};

    shaderMgr.ReadShadersAsync(options, 2);
    shaderMgr.ReadShadersAsync(options, 2);

    TextureManager& textureMgr = TextureMgrInstance();
    TextureOption textureOption(TextureType::TEXTURE_2D);
    textureMgr.ReadTextureAsync("Assets/Textures/skybox/front.png", &textureOption);
    textureMgr.ReadTextureAsync("Assets/Textures/skybox/front.png", &textureOption);

    MainWindow_.Run();
};