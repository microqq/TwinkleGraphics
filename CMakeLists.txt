cmake_minimum_required(VERSION 3.0.0)
project(TwinkleGraphics VERSION 0.1.0)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    find_package(LLVM REQUIRED CONFIG)
    message(STATUS "Found LLVM ${LLVM_PACKAGE_VERSION}")
    message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    # FIND_LIBRARY(COMM_LIB comm "D:/Program Files/glfw-3.3.bin.WIN64/lib-mingw-w64" NO_DEFAULT_PATH)
    # FIND_LIBRARY(RUNTIME_LIB rt "D:/Program Files/glfw-3.3.bin.WIN64/lib-mingw-w64" NO_DEFAULT_PATH)
endif()


SET(GLEWDIR D:/YYStudio/glew-2.1.0/lib/Release/x64)
SET(GLFWDIR D:/YYStudio/glfw-3.3.bin.WIN64/lib-mingw-w64)

# # CTest
# include(CTest)
# enable_testing()

# set output push
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output/libs)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output)

# including path 
include_directories([AFTER|BEFORE] [SYSTEM] 
    ${PROJECT_SOURCE_DIR}/ThirdParty 
    ${PROJECT_SOURCE_DIR}/Common
    ${PROJECT_SOURCE_DIR}/TwinkleShading
)

# add subdirectory
add_subdirectory(Common)
add_subdirectory(TwinkleShading)
add_subdirectory(TwinkleRayTracing)

# # CPACK
# set(CPACK_PROJECT_NAME ${PROJECT_NAME})
# set(CPACK_PROJECT_VERSION ${PROJECT_VERSION})
# include(CPack)
