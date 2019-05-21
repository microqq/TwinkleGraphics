cmake_minimum_required(VERSION 3.0.0)
project(TwinkleGraphics VERSION 0.1.0)

find_package(LLVM REQUIRED CONFIG)

include(CTest)
enable_testing()

## including path 
include_directories([AFTER|BEFORE] [SYSTEM] ${PROJECT_SOURCE_DIR}/ThirdParty)

## add executeable 
add_executable(TwinkleShading ${PROJECT_SOURCE_DIR}/twShading.cpp)
target_link_libraries(TwinkleShading -lglfw -lGL -lGLEW)

## set output push
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output/libs)

set(CPACK_PROJECT_NAME ${PROJECT_NAME})
set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
include(CPack)
