cmake_minimum_required(VERSION 3.0.0)
project(TwinkleGraphics VERSION 0.1.0)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    find_package(LLVM REQUIRED CONFIG)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
endif()

# # CTest
# include(CTest)
# enable_testing()

## including path 
include_directories([AFTER|BEFORE] [SYSTEM] ${PROJECT_SOURCE_DIR}/ThirdParty)

## add executeable 
add_executable(TwinkleShading ${PROJECT_SOURCE_DIR}/twShading.cpp)
target_link_libraries(TwinkleShading -lglfw -lGL -lGLEW)

## set output push
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output/libs)

# # CPACK
# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
