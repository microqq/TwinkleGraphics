cmake_minimum_required(VERSION 3.8.2)
project(TwinkleGraphics VERSION 0.1.0)

option(ENABLE_DEBUG "Enter debug mode" ON)
option(ENABLE_TEMPORARY "Enter debug mode" OFF)
option(ENALBE_EVENT_MULTTHREAD "Enable event multithreading mode" ON)
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

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output/libs)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    SET(GLEWLIBDIR $ENV{GLEW_LIB_PATH})
    SET(GLFWLIBDIR $ENV{GLFW_LIB_PATH})
    SET(GTESTINCLUDEDIR $ENV{GTESTINCLUDE})
    SET(GTESTLIBDIR $ENV{GTESTLIB})
    
    include_directories([AFTER|BEFORE] [SYSTEM]
        ${PROJECT_SOURCE_DIR}/ThirdParty/dirent/include
    )    
endif()

# including path
include_directories([AFTER|BEFORE] [SYSTEM]
    ${PROJECT_SOURCE_DIR}/ThirdParty
    ${PROJECT_SOURCE_DIR}/ThirdParty/vermilion
    ${PROJECT_SOURCE_DIR}/Iinclude/Common
    ${PROJECT_SOURCE_DIR}/Include/UI
    ${PROJECT_SOURCE_DIR}/Include/Util
    ${PROJECT_SOURCE_DIR}/Include/Event
    ${PROJECT_SOURCE_DIR}/TwinkleShading
)

# add subdirectory
add_subdirectory(ThirdParty/imgui)
add_subdirectory(ThirdParty/ImGuiFileDialog)
add_subdirectory(ThirdParty/vermilion)
add_subdirectory(Source)
add_subdirectory(TwinkleShading)
add_subdirectory(TwinkleRayTracing)

if(BUILD_GTESTS)
    add_subdirectory(Testbeds/GTests)
    
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})
endif()

    # add_test(UniformSetting ${EXECUTABLE_OUTPUT_PATH}/MaterialTests)
    # add_test(AddEventHandler ${EXECUTABLE_OUTPUT_PATH}/EventTests)
    # add_test(FireEvent ${EXECUTABLE_OUTPUT_PATH}/EventTests)
    # enable_testing()
endif()