### 问题汇总


#### 1、Vcpkg 安装第三方依赖库时速度很慢

    解决办法：例如配置 TwinkleGraphics 项目时，需要安装 assimp，执行 cmake 自动调用 vcpkg 安装时发现速度非常慢，这时候可以查看 assimp 的下载地址时什么，直接复制到浏览器下载，将下载后的包改好名字后放到vcpkg下的downloads文件夹，这样安装会快很多。

#### 2、cmake进入Verbose mode

    解决办法：执行cmake 时加上命令 -DCMAKE_VERBOSE_MAKEFILE=ON


#### 3、错误提示pkg-config not found
    解决办法：通过vcpkg 安装pkg-config：

```bash
    vcpkg install pkgconf:x64-mingw-static
    ## x64-mingw-static 指定平台和编译器，也可以使用其他平台如x64-uwp、x86-windows等。
```

#### 4、vcpkg.json
    vcpkg.json 在编写时必须注意，dependencies 不能填写版本，应在 overrides 中填写具体的版本，但是 features 则有不同，可以直接在其中指定一个或者多个 feature，如：

```json
    "dependencies": [
        {
            "name": "glew"
        },
        {
            "name": "imgui",
            "features": [
                "docking-experimental",
                "glfw-binding",
                "opengl3-binding"
            ]
        }
    ],
    "overrides": [
        {
            "name": "freetype",
            "version": "2.13.3"
        },
        {
            "name": "assimp",
            "version": "5.4.3"
        }
    ],
```

#### 5、vcpkg builtin-baseline 的问题
    问题：如果 vcpkg.json 里没有这个字段， 那么在没有任何 registries 配置过的 manifest 中是没法运行 vcpkg install 或执行 cmake 编译的， 会报错的

    解决办法：执行 vcpkg x-update-baseline 命令的 --add-initial-baseline 选项用来向 vcpkg.json 中添加一个 "builtin-baseline" 字段


#### 6、cmake 编译时指定生成器类型
    cmake -G 命令用来指定生成器类型，也就是不同的编译器所需要的不同的构建文件，如使用Mingw工具链的生成器：
```bash
    cmake -G "MinGW Makefiles"
    # 这是指定使用Mingw64（GCC）来编译
```
    其他还有：
    1) Unix Makefiles：适用于 Unix 系统（包括 Linux 和 macOS）的标准 Makefile 生成。
    2) Ninja：一种快速的构建系统。
    3) Visual Studio <version>：例如 Visual Studio 17 2022，用于生成特定版本的 Visual Studio 项目文件。
    4) Xcode：用于生成 macOS 上的 Xcode 项目。

    当然还有其他更多的生成器类型，具体可通过 cmake --help 查看


#### 7、cmake + vcpkg 的基本步骤

1. 安装 Vcpkg
   * 按照 Vcpkg 的官方文档中的说明进行安装。

2. 初始化 Vcpkg
   * 打开命令提示符或终端，导航到 Vcpkg 所在的目录，并运行 .\vcpkg integrate install 命令来初始化 Vcpkg 与 CMake 的集成。

3. 创建 CMake 项目
   * 创建一个新的 CMake 项目，并在项目的根目录下创建一个 CMakeLists.txt 文件。

4. 在 CMakeLists.txt 中设置 Vcpkg

    ```cmake
        cmake_minimum_required(VERSION 3.0)

        # 设置 Vcpkg 工具链
        set(VCPKG_FEATURE_FLAGS "version")
        set(CMAKE_TOOLCHAIN_FILE "<vcpkg_root>/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")
        set(VCPKG_TARGET_TRIPLET "x64-mingw-static")
        set(VCPKG_HOST_TRIPLET "x64-mingw-static")


        # 项目名称
        project(YourProjectName)

        # 查找依赖库
        find_package(YourDependency REQUIRED)
    ```
   * 将 <vcpkg_root> 替换为您实际安装 Vcpkg 的路径。YourDependency 替换为您要安装的具体依赖库的名称。

5. 配置和生成 CMake 项目
   * 在项目根目录下打开命令提示符或终端，运行 CMake 命令来配置项目，例如：
    ```bash
        cmake -S. -B build
    ```

6. 构建项目
   * 根据您的操作系统和生成器类型，在生成的 build 目录中执行相应的构建命令，例如在 Windows 上使用 Visual Studio 生成器可能是：

    ```
        cmake --build build --config Release
    ```

通过以上步骤，您应该能够使用 Vcpkg 在 CMake 项目中安装和管理第三方依赖库。请注意，具体的依赖库名称和配置可能因您的项目需求而有所不同。

#### 8、每一种工具都可能有不止一种使用方式，我们还可以配置vcpkg.json文件来指定第三方依赖库

以下是使用 `vcpkg.json` 配置并结合 CMake 自动安装第三方依赖库的详细步骤：

1. 安装 Vcpkg
    - 按照 Vcpkg 的官方文档进行安装。

2. 创建 `vcpkg.json` 配置文件
    - 在项目根目录下创建一个名为 `vcpkg.json` 的文件。
    - 在 `vcpkg.json` 中指定所需的依赖库，例如：
    ```json
    {
        "name": "YourProjectName",
        "dependencies": [
            "openssl",
            "zlib"
        ]
    }
    ```
    将 `YourProjectName` 替换为您的项目名称，`openssl` 和 `zlib` 替换为您实际需要的依赖库名称。

3. 创建 CMake 项目
    - 在项目根目录下创建一个 `CMakeLists.txt` 文件。

4. 在 `CMakeLists.txt` 中设置 Vcpkg
    ```cmake
    cmake_minimum_required(VERSION 3.0)

    # 设置 Vcpkg 工具链
    set(CMAKE_TOOLCHAIN_FILE "<vcpkg_root>/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

    # 项目名称
    project(YourProjectName)

    # 执行 Vcpkg 安装
    execute_process(
        COMMAND <vcpkg_root>/vcpkg install
        WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    )
    ```
    将 `<vcpkg_root>` 替换为您实际安装 Vcpkg 的路径。

5. 配置和生成 CMake 项目
    - 在项目根目录下打开命令提示符或终端，运行 CMake 命令来配置项目，例如：
    ```
    cmake -S. -B build
    ```

6. 构建项目
    - 根据您的操作系统和生成器类型，在生成的 `build` 目录中执行相应的构建命令。
  
7. execute_process 的使用问题
   
    通常情况下，确实不需要在 `CMakeLists.txt` 中添加上述的 `execute_process` 命令来安装依赖。

    当您在项目根目录下设置好了 `vcpkg.json` ，并且在 `CMakeLists.txt` 中正确设置了 Vcpkg 的工具链：

    ```cmake
    cmake_minimum_required(VERSION 3.0)

    # 设置 Vcpkg 工具链
    set(CMAKE_TOOLCHAIN_FILE "<vcpkg_root>/scripts/buildsystems/vcpkg.cmake" CACHE STRING "Vcpkg toolchain file")

    # 项目名称
    project(YourProjectName)
    ```

    然后执行 CMake 配置命令（例如 `cmake -S. -B build` ）时，CMake 会根据 `vcpkg.json` 中的依赖配置自动处理依赖的安装。

    但在某些特殊情况下，如果 CMake 没有正确识别或处理 `vcpkg.json` 中的依赖，您可能需要手动添加类似上述的命令来强制安装依赖。不过这种情况相对较少见。

这样，当您执行 CMake 配置命令时，它会自动调用 Vcpkg 安装在 `vcpkg.json` 中指定的依赖库。


#### 9、很多问题这个链接讲的比较清楚：https://blog.csdn.net/aqwca/article/details/142185958


#### 10、How to use static and dynamic libraries at the same time?

* [build custom triplets](https://github.com/microsoft/vcpkg/discussions/33256)
* [Per-port customization](https://learn.microsoft.com/en-us/vcpkg/users/triplets#per-port-customization)
* [Managing dependencies in a C++ project with vcpkg](https://decovar.dev/blog/2022/10/30/cpp-dependencies-with-vcpkg/#why-we-didnt-proceed-with-conan)
* [xmake与包管理:又一个现代c++构建工具?实际是介绍vcpkg在cmake中的应用](https://blog.csdn.net/aqwca/article/details/142185958)
* [x86-windows-mixed](https://github.com/Intelight/vcpkg/blob/master/triplets/x86-windows-mixed.cmake)

#### 11、overlay triplets

有两种方式使用overlay triplets，一是直接命令使用，如：
```bash
~/git$ mkdir custom-triplets
~/git$ cp vcpkg/triplets/x64-linux.cmake custom-triplets/x64-linux-dynamic.cmake
~/git$ vcpkg/vcpkg install sqlite3:x64-linux-dynamic --overlay-triplets=custom-triplets
```

第二种方式manifest mode，在根目录下，CMakeLists.txt 中 Project(xxx) 执行之前添加命令：

```cmake
set(VCPKG_FEATURE_FLAGS "version")
set(VCPKG_OVERLAY_TRIPLETS "$ENV{VCPKG_ROOT}/custom-triplets")
set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic-openexr")
set(VCPKG_HOST_TRIPLET "x64-mingw-dynamic-openexr")
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")
```