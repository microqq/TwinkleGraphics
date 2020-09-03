cmake_minimum_required(VERSION 3.8.2)
project(TwinkleGraphics VERSION 0.1.0)

option(USE_DEBUG "Enter debug mode" OFF)
option(ENABLE_TEMPORARY "Enter debug mode" OFF)
Option(ENALBE_EVENT_MULTTHREAD "Enable event multithreading mode" OFF)
OPTION(ENABLE_EXPORT_DLL "build dynamic library" ON)

if(ENALBE_EVENT_MULTTHREAD)
    add_definitions(-D_EVT_MULTTHREAD)
endif()

if(USE_DEBUG)
    add_definitions(-D_DEBUG)
endif()

if(ENABLE_TEMPORARY)
    add_definitions(-DTEMPORARY_USE)
endif()

if(ENABLE_EXPORT_DLL)
    add_definitions(-DEXPORT_DLL)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    find_package(LLVM REQUIRED CONFIG)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
endif()

# set everything up for c++ 17 features
set(CMAKE_CXX_STANDARD 17)
# Don't add this line if you will try_compile with boost.
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# include(CheckCXXSymbolExists)
# CHECK_CXX_SYMBOL_EXISTS(std::filesystem::path::preferred_separator filesystem cxx17fs)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    SET(GLEWLIBDIR $ENV{GLEW_LIB_PATH})
    SET(GLFWLIBDIR $ENV{GLFW_LIB_PATH})
    SET(GTESTINCLUDEDIR $ENV{GTESTINCLUDE})
    SET(GTESTLIBDIR $ENV{GTESTLIB})
    set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output/libs)
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
endif()

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output)

# including path
include_directories([AFTER|BEFORE] [SYSTEM]
    ${PROJECT_SOURCE_DIR}/ThirdParty
    ${PROJECT_SOURCE_DIR}/ThirdParty/dirent/include
    ${PROJECT_SOURCE_DIR}/ThirdParty/vermilion
    ${PROJECT_SOURCE_DIR}/Iinclude/Common
    ${PROJECT_SOURCE_DIR}/Include/UI
    ${PROJECT_SOURCE_DIR}/Include/Event
    ${PROJECT_SOURCE_DIR}/TwinkleShading
)

# add subdirectory
add_subdirectory(ThirdParty/imgui)
add_subdirectory(ThirdParty/ImGuiFileDialog)
add_subdirectory(ThirdParty/vermilion)
add_subdirectory(Source/Common)
add_subdirectory(Source/UI)
add_subdirectory(Source/Event)
add_subdirectory(TwinkleShading)
add_subdirectory(TwinkleRayTracing)

OPTION(BUILD_GTESTS "build gtests" OFF)
IF(BUILD_GTESTS)
    add_subdirectory(Testbeds/GTests)
    
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
endif()

    add_test(UniformSetting ${EXECUTABLE_OUTPUT_PATH}/MaterialTests)
    add_test(AddEventHandler ${EXECUTABLE_OUTPUT_PATH}/EventTests)
    add_test(FireEvent ${EXECUTABLE_OUTPUT_PATH}/EventTests)

    enable_testing()
ENDIF(BUILD_GTESTS)