cmake_minimum_required(VERSION 3.8.2)
cmake_policy(VERSION 3.8.2)

project(TwinkleGraphics VERSION 0.1.0)

option(ENABLE_DEBUG "Enter debug mode" ON)
option(ENABLE_TEMPORARY "Enter debug mode" OFF)
option(ENALBE_EVENT_MULTTHREAD "Enable event multithreading mode" OFF)
option(BUILD_GTESTS "build gtests" ON)

if(ENALBE_EVENT_MULTTHREAD)
    add_definitions(-D_EVT_MULTTHREAD)
endif()

if(ENABLE_DEBUG)
    add_definitions(-D_DEBUG)
endif()

if(ENABLE_TEMPORARY)
    add_definitions(-DTEMPORARY_USE)
endif()

add_definitions(-DCUSTOM_IMGUIFILEDIALOG_CONFIG="${CMAKE_CURRENT_LIST_DIR}/Include/UI/twCustomImGuiFileDialogConfig.h")

# set everything up for c++ 17 features
set(CMAKE_CXX_STANDARD 17) # this does nothing for MSVC, use target_compile_options below
# Don't add this line if you will try_compile with boost.
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

# include(CheckCXXSymbolExists)
# CHECK_CXX_SYMBOL_EXISTS(std::filesystem::path::preferred_separator filesystem cxx17fs)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_CURRENT_LIST_DIR}/cmake/modules/")

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output/libs)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output)

SET(FREEIMAGEINCLUDEDIR $ENV{FREEIMAGE_INCLUDE_PATH})
SET(FREEIMAGELIBDIR $ENV{FREEIMAGE_LIB_PATH})
SET(ASSIMPINCLUDEDIR $ENV{ASSIMP_INCLUDE_PATH})
SET(ASSIMPLIBDIR $ENV{ASSIMP_LIB_PATH})
SET(GLEWINCLUDEDIR $ENV{GLEW_INCLUDE_PATH})
SET(GLEWLIBDIR $ENV{GLEW_LIB_PATH})
SET(GLFWINCLUDEDIR $ENV{GLFW_INCLUDE_PATH})
SET(GLFWLIBDIR $ENV{GLFW_LIB_PATH})
SET(GTESTINCLUDEDIR $ENV{GTEST_INCLUDE_PATH})
SET(GTESTLIBDIR $ENV{GTEST_LIB_PATH})
SET(DIRENTINCLUDEDIR ${CMAKE_CURRENT_LIST_DIR}/ThirdParty/includes/dirent)

# find the required packages
# find_package(GLM REQUIRED)
# message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLEW REQUIRED)
message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")
find_package(FREEIMAGE REQUIRED)
message(STATUS "Found FREEIMAGE in ${FREEIMAGE_INCLUDE_DIR}")

if(APPLE)
    find_package(Freetype REQUIRED)
    message(STATUS "Found Freetype in ${FREETYPE_INCLUDE_DIRS}")
endif(APPLE)

# set include directory in MacOS platform
if(APPLE)
    INCLUDE_DIRECTORIES(/System/Library/Frameworks ${CMAKE_CURRENT_LIST_DIR}/Include)
endif(APPLE)

# find library
if(APPLE)
    FIND_LIBRARY(COCOA_LIBRARY Cocoa)
    FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
    FIND_LIBRARY(IOKit_LIBRARY IOKit)
    FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
    MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
endif(APPLE)

# if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
if(LINUX)
    find_package(LLVM REQUIRED CONFIG)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
endif()

# set including path
include_directories([AFTER|BEFORE] [SYSTEM]
    ${PROJECT_SOURCE_DIR}/ThirdParty/includes
    ${PROJECT_SOURCE_DIR}/ThirdParty/vermilion
    ${PROJECT_SOURCE_DIR}/ThirdParty/imgui
    ${PROJECT_SOURCE_DIR}/ThirdParty/ImGuiFileDialog
    ${PROJECT_SOURCE_DIR}/Iinclude/Common
    ${PROJECT_SOURCE_DIR}/Include/UI
    ${PROJECT_SOURCE_DIR}/Include/UI/GLFW
    ${PROJECT_SOURCE_DIR}/Include/Util
    ${PROJECT_SOURCE_DIR}/Include/Event
    ${PROJECT_SOURCE_DIR}/TwinkleShading
)

# set link directoriew
link_directories(
    ${FREEIMAGELIBDIR}
    ${ASSIMPLIBDIR}
    ${GLEWLIBDIR}
    ${GLFWLIBDIR}
)

# add subdirectory
add_subdirectory(ThirdParty)
add_subdirectory(Source)
# add_subdirectory(TwinkleShading)
# add_subdirectory(TwinkleRayTracing)

if(BUILD_GTESTS)
    include_directories(${GTESTINCLUDEDIR})
    link_directories(${GTESTLIBDIR})
    # add_subdirectory(Testbeds/GTests)
    
# if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
if(LINUX)
    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)
    include_directories(${GTESTINCLUDEDIR})
endif(LINUX)
endif(BUILD_GTESTS)