
#include <functional>
#include <string>
#include <gtest/gtest.h>

#include "twEventHandler.h"
#include "twEventManager.h"
#include "twConsoleLog.h"
#include "twResource.h"
#include "twShader.h"

using namespace TwinkleGraphics;
using namespace std::chrono_literals;

template <typename R>
bool IsReady(std::future<R> const &f)
{
    return f.wait_for(std::chrono::microseconds(200ms)) == std::future_status::ready;
}

TEST(ResourceReaderTests, Read)
{

};

ShaderOption option(ShaderOption::OptionData{ "Assets/Shaders/line.vert", ShaderType::VERTEX_SHADER });

TEST(ResourceReaderTests, ReadAsync)
{
    TextManagerInst textMgr;
    textMgr->ReadTextAsync("Assets/Shaders/line.vert");

    ShaderManagerInst shaderMgr;
    shaderMgr->ReadShaderAsync("Assets/Shaders/line.vert", &option);
};