### 1. 目的

给出一个最小的工程， 展示如何使用特定版本的依赖库。

具体说， 使用 9.1.0 版本的 fmtlib （而非最新的 10.0.0 版本），打印输出 hello world。

### 2. 常规部分

这部分是一个基本的 CMake-based C++ Hello World 工程， 虽然有 CMakeLists.txt, 但并不涉及 vcpkg 的使用。如果你有经验， 这一节可以跳过。
#### 2.1 基本环境

操作系统

vcpkg 对于常见的库， 可以认为覆盖了 Windows， Linux， MacOSX 的 x64 环境， 操作系统用哪个应该都可以， 本人使用的是 ubuntu 22.04。

CMake版本

本人使用较新的 CMake 3.27.4 版本，稍微老一些的如3.20应该也可以。

vcpkg版本

2023-08-09-9990a4c9026811a312cb2af78bf77f3d9d288416

#### 2.2 C++ 代码

只有一个 main.cpp

```
#include <fmt/core.h>

int main()
{
    fmt::print("Hello World\n");
    return 0;
}
```

#### 2.3 CMakeLists.txt

```
cmake_minimum_required(VERSION 3.20)

project(kys.cpp)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_DEBUG_POSTFIX "_d")

find_package(fmt CONFIG REQUIRED)

add_executable(kys
  kys.cpp
)

target_link_libraries(kys PRIVATE fmt::fmt)
```

### 3. vcpkg 部分
#### 3.1 manifest mode

中文翻译为 “清单模式”, 是相对于早期 vcpkg 的“经典模式”的新模式， 官方推荐使用 manifest 模式。 一种典型场景是： 当前电脑上有两个工程， 依赖了不同版本的 fmtlib， 如分别是 9.1.0 和 10.0.0 版本， 如果是经典模式， 似乎本机只能存在一个版本的 fmtlib， manifest 模式则看起来可以让多个版本共存。

#### 3.2 初版 vcpkg.json - 手写

通常的习惯是， 在项目根目录放置 CMakeLists.txt。 在这个前提下， 在工程根目录创建名为 vcpkg.json 的文件， 内容为：

{
  "name": "kys-cpp",
  "version-semver": "0.0.0",
  "dependencies": [
    "fmt"
  ],
  "overrides": [
    {
      "name": "fmt",
      "version": "9.1.0"
    }
  ]
}

其中:

    dependencies 字段列出了依赖库（只有一个， fmt );
    overrides 字段通常被教程讲解为“可选字段”，但个人认为它更应当被作为“必选字段”，用来指定每个依赖库的具体版本

到这里，并不是完整的 vcpkg.json, 原因是当你执行

vcpkg install

会提示:

error: /home/zz/work/kys.cpp/vcpkg.json was rejected because it uses "overrides" and does not have a "builtin-baseline". This can be fixed by removing the uses of "overrides" or adding a "builtin-baseline".
See `vcpkg help versioning` for more information.

需要我们提供 "builtin-baseline" 字段和合适的取值。

#### 3.3 生成 builtin-baseline (重点)

在工程根目录下执行：

vcpkg x-update-baseline --add-initial-baseline

查看 vcpkg.json, 发现自动添加了内容：

"builtin-baseline": "da21e45c9ae689f63aa27f3a4ee0e64fe6b16494"

完整内容:

{
  "name": "kys-cpp",
  "version-semver": "0.0.0",
  "dependencies": [
    "fmt"
  ],
  "overrides": [
    {
      "name": "fmt",
      "version": "9.1.0"
    }
  ],
  "builtin-baseline": "da21e45c9ae689f63aa27f3a4ee0e64fe6b16494"
}

原始讨论帖: https://github.com/microsoft/vcpkg/discussions/25622

官方最新文档: https://learn.microsoft.com/en-us/vcpkg/commands/update-baseline

    Synopsis
    vcpkg x-update-baseline [options] [--add-initial-baseline] [--dry-run]
    Description
    Update baselines for all configured registries.
    In Manifest mode, this command operates on all registries in the vcpkg.json and the vcpkg-configuration.json. In Classic mode, this command operates on the vcpkg-configuration.json in the vcpkg instance ($VCPKG_ROOT).
    See the versioning documentation for more information about baselines.
    Options
    --add-initial-baseline
    Add a "builtin-baseline" field to the vcpkg.json if it does not already have one.
    Without this flag, it is an error to run this command on a manifest that does not have any registries configured.

加粗部分解释的挺清楚：vcpkg x-update-baseline 命令的 --add-initial-baseline 选项用来向 vcpkg.json 中添加一个 "builtin-baseline" 字段（如果还没存在这个字段的话）。如果 vcpkg.json 里没有这个字段， 那么在没有任何 registries 配置过的 manifest 中是没法运行 vcpkg install 或执行 cmake 编译的， 会报错的， 报错内容即是本文 3.2 小节末尾的内容。
#### 3.4 执行 cmake 构建， 导入依赖库

export VCPKG_ROOT=/home/zz/work/vcpkg # 改为你的实际安装路径
cmake -B build -S . -DCMAKE_TOOLCHAIN_FILE=${VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake
cmake --build build
cd build
./kys.cpp

将会输出:

    Hello, World

### 4. 总结

使用 vcpkg 导入依赖库，和常规的使用 CMake 并不冲突， 而是一种补充。

使用 vcpkg 的 manifest 模式， 和 class 模式并不冲突， 是一种更灵活的配置方式。

使用 vcpkg manifest 模式时， 官方文档似乎刻意隐藏 “指定特定依赖库的特定版本” 的写法， 这不太友好 （见 github issue 中围观者的 downvote 数量）； 只需要两步就可以“友好”：

1） 增加 "overrides" 字段来指明依赖库的特定版本， 并于 dependencies 匹配：

"dependencies": [
    "fmt"
  ],
  "overrides": [
    {
      "name": "fmt",
      "version": "9.1.0"
    }
  ]

2）执行 vcpkg x-update-baseline --add-initial-baseline 来生成和自动填充 "builtin-baseline" 字段:

"builtin-baseline": "da21e45c9ae689f63aa27f3a4ee0e64fe6b16494"

然后， 按常规的 CMake 工作流执行， 即可完成程序的构建（至于 CMake find_package 怎么写， 其实 vcpkg 很友好的在命令行中有提示输出， 见 vcpkg install 的输出， 拷贝使用即可）。