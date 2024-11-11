cmake_minimum_required(VERSION 3.30.0)
cmake_policy(VERSION 3.30.0)

# set vcpkg options
set(VCPKG_FEATURE_FLAGS "version")
set(VCPKG_OVERLAY_TRIPLETS "$ENV{VCPKG_ROOT}/custom-triplets")
set(VCPKG_TARGET_TRIPLET "x64-mingw-dynamic-openexr")
set(VCPKG_HOST_TRIPLET "x64-mingw-dynamic-openexr")
set(CMAKE_TOOLCHAIN_FILE $ENV{VCPKG_ROOT}/scripts/buildsystems/vcpkg.cmake CACHE STRING "Vcpkg toolchain file")

# failed to find pkg-config, so we set it manually.
# 1\install pkg-config manually
# 2\set variable
set(PKG_CONFIG_EXECUTABLE $ENV{VCPKG_ROOT}/installed/x64-mingw-dynamic-openexr/tools/pkgconf/pkgconf.exe)

# set project version
project(TwinkleGraphics VERSION 0.2.0)

# set project platform and compiler options
SET(CMAKE_SYSTEM_NAME Windows)
SET (CMAKE_C_FLAGS                "-Wall -std=c11")
SET (CMAKE_C_FLAGS_DEBUG          "-g -O0")
SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")
 
SET (CMAKE_CXX_FLAGS   "-std=c++17")
SET (CMAKE_CXX_FLAGS   "-g")
# SET (CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -Wall -Wextra")
SET (CMAKE_CXX_FLAGS_DEBUG   "-g -O0" )
SET (CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG " )

# set everything up for c++ 17 features
set(CMAKE_CXX_STANDARD 17) # this does nothing for MSVC, use target_compile_options below

# Don't add this line if you will try_compile with boost.
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# set build type
if(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug cache string "Choose the type of build (Debug or Release)" force)
endif(NOT CMAKE_BUILD_TYPE)

# options switch
option(ENABLE_DEBUG "Enter debug mode" ON)
option(ENABLE_TEMPORARY "Enter debug mode" OFF)
option(ENALBE_EVENT_MULTTHREAD "Enable event multithreading mode" OFF)
option(BUILD_GTESTS "build gtests" ON)
option(ENABLE_EXPORT_PLUGIN "Enable export plugin" ON)
option(ENABLE_EXPORT_COMDLL "build common dynamic library" ON)
option(DEFINE_FREEIMAGE_LIB "use static freeimage lib" OFF)

if(ENALBE_EVENT_MULTTHREAD)
    add_definitions(-D_EVT_MULTTHREAD)
endif()

if(ENABLE_DEBUG)
    add_definitions(-D_DEBUG)
endif()

if(ENABLE_TEMPORARY)
    add_definitions(-DTEMPORARY_USE)
endif()

if(ENABLE_EXPORT_PLUGIN)
    add_definitions(-DEXPORT_PLUGIN)
endif()

if(ENABLE_EXPORT_COMDLL)
    add_definitions(-DEXPORT_COMDLL)
endif()

if(DEFINE_FREEIMAGE_LIB)
    add_definitions(-DFREEIMAGE_LIB)
endif()

# definitions
add_definitions(-DCUSTOM_IMGUIFILEDIALOG_CONFIG="${CMAKE_CURRENT_LIST_DIR}/Include/UI/twCustomImGuiFileDialogConfig.h")
add_definitions(-DIMGUI_DEFINE_MATH_OPERATORS=1)
add_definitions(-DIMGUI_IMPL_OPENGL_LOADER_GLEW=1)

# include(CheckCXXSymbolExists)
# CHECK_CXX_SYMBOL_EXISTS(std::filesystem::path::preferred_separator filesystem cxx17fs)

# # cmake module path(FindXXX.cmake) [deprecated]
# set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output)

# find the required packages
find_package(glm CONFIG REQUIRED)
find_package(glew CONFIG REQUIRED)
find_package(glfw3 CONFIG REQUIRED)
find_package(assimp CONFIG REQUIRED)
find_package(imgui CONFIG REQUIRED)
find_package(freeImage CONFIG REQUIRED)

# if(WIN32)
#     find_library(imgui REQUIRED_HINTS "${PROJECT_SOURCE_DIR}/build/vcpkg_installed/x64-mingw-dynamic-openexr/lib" NAMES imgui)
# endif()

if(APPLE)
    find_package(Freetype REQUIRED)
    message(STATUS "Found Freetype in ${FREETYPE_INCLUDE_DIRS}")
endif(APPLE)

# find library
if(APPLE)
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
    FIND_LIBRARY(IOKit_LIBRARY IOKit)
    FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
    MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
endif(APPLE)

if(LINUX)
    find_package(LLVM REQUIRED CONFIG)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
endif()

# set include directory in MacOS platform
if(APPLE)
    INCLUDE_DIRECTORIES(/System/Library/Frameworks ${CMAKE_CURRENT_LIST_DIR}/Include)
endif(APPLE)

# set including path
include_directories([AFTER|BEFORE] [SYSTEM]
    ${PROJECT_SOURCE_DIR}/build/vcpkg_installed/x64-mingw-dynamic-openexr/include
    ${PROJECT_SOURCE_DIR}/ThirdParty/ImGuiFileDialog
    ${PROJECT_SOURCE_DIR}/Iinclude/Common
    ${PROJECT_SOURCE_DIR}/Include/UI
    ${PROJECT_SOURCE_DIR}/Include/Editor
    ${PROJECT_SOURCE_DIR}/Include/Editor/ImGui
    ${PROJECT_SOURCE_DIR}/Include/Editor/GLFW
    ${PROJECT_SOURCE_DIR}/Include/Editor/ImGuiDocking
    ${PROJECT_SOURCE_DIR}/Include/Util
    ${PROJECT_SOURCE_DIR}/Include/Event
    ${PROJECT_SOURCE_DIR}/TwinkleShading
)

# set link directories
link_directories(
    ${PROJECT_SOURCE_DIR}/build/vcpkg_installed/x64-mingw-dynamic-openexr/bin
    ${PROJECT_SOURCE_DIR}/build/vcpkg_installed/x64-mingw-dynamic-openexr/lib
    # ${PROJECT_SOURCE_DIR}/build/vcpkg_installed/x64-mingw-dynamic-openexr/debug/bin
    # ${PROJECT_SOURCE_DIR}/build/vcpkg_installed/x64-mingw-dynamic-openexr/debug/lib
    ${PROJECT_SOURCE_DIR}/Output
    ${PROJECT_SOURCE_DIR}/Output/libs
    ${PROJECT_SOURCE_DIR}/Output/libs/Debug
)

# add subdirectory
add_subdirectory(ThirdParty)
add_subdirectory(Source)
add_subdirectory(TwinkleShading)
# add_subdirectory(TwinkleRayTracing)


# build gtests options
if(BUILD_GTESTS)
    include_directories(${GTESTINCLUDEDIR})
    link_directories(${GTESTLIBDIR})
    # add_subdirectory(Testbeds/GTests)
if(LINUX)
    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)
    include_directories(${GTESTINCLUDEDIR})
endif(LINUX)
endif(BUILD_GTESTS)