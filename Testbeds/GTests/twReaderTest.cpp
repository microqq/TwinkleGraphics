
#include <functional>
#include <string>
#include <gtest/gtest.h>

#include "twConsoleLog.h"
#include "twResource.h"
#include "twShader.h"
#include "twMainWindow.h"
#include "twResourceManager.h"

using namespace TwinkleGraphics;
using namespace std::chrono_literals;

ShaderOption ShaderOption_(ShaderOption::OptionData{ "Assets/Shaders/line.vert", ShaderType::VERTEX_SHADER });

TEST(ResourceReaderTests, Read)
{
    ShaderManager& shaderMgr = ShaderMgrInstance();
    shaderMgr.ReadShader("Assets/Shaders/line.vert", &ShaderOption_);
};

TEST(ResourceReaderTests, ReadAsync)
{
    GLFWMainWindow MainWindow_(1024, 768);

    ShaderManager& shaderMgr = ShaderMgrInstance();
    shaderMgr.ReadShaderAsync("Assets/Shaders/line.vert", &ShaderOption_);

    MainWindow_.Run();
};