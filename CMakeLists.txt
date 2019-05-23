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

# set output push
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output)
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/Output/libs)

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
