cmake_minimum_required(VERSION 3.8.2)
project(TwinkleGraphics VERSION 0.1.0)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    find_package(LLVM REQUIRED CONFIG)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
endif()

# set everything up for c++ 17 features
set(CMAKE_CXX_STANDARD 17)
# Don't add this line if you will try_compile with boost.
set(CMAKE_CXX_STANDARD_REQUIRED ON)


if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    # FIND_LIBRARY(COMM_LIB comm "D:/Program Files/glfw-3.3.bin.WIN64/lib-mingw-w64" NO_DEFAULT_PATH)
    # FIND_LIBRARY(RUNTIME_LIB rt "D:/Program Files/glfw-3.3.bin.WIN64/lib-mingw-w64" NO_DEFAULT_PATH)
endif()


if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    SET(GLEWLIBDIR $ENV{GLEW_LIB_PATH})
    SET(GLFWLIBDIR $ENV{GLFW_LIB_PATH})
elseif(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
endif()


if(${CMAKE_BUILD_TYPE} STREQUAL "Debug")
endif()


# set output push
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output/libs)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output)

# including path
include_directories([AFTER|BEFORE] [SYSTEM]
    ${PROJECT_SOURCE_DIR}/ThirdParty
    ${PROJECT_SOURCE_DIR}/ThirdParty/vermilion
    ${PROJECT_SOURCE_DIR}/Common
    ${PROJECT_SOURCE_DIR}/TwinkleShading
)

# add subdirectory
add_subdirectory(ThirdParty/imgui)
add_subdirectory(ThirdParty/vermilion)
add_subdirectory(Common)
add_subdirectory(TwinkleShading)
add_subdirectory(TwinkleRayTracing)

OPTION(BUILD_GTESTS "build gtests" OFF)
IF(BUILD_GTESTS)
    find_package(GTest REQUIRED)
    find_package(Threads REQUIRED)
    include_directories(${GTEST_INCLUDE_DIRS})

    add_subdirectory(Testbeds/GTests)
    add_test(UniformSetting ${EXECUTABLE_OUTPUT_PATH}/MaterialTests)
    enable_testing()
ENDIF(BUILD_GTESTS)