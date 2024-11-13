
cmake_minimum_required(VERSION 3.0)
cmake_policy(VERSION 3.0)

project(GLFWTestOSX)

set(CMAKE_CXX_STANDARD 17) # this does nothing for MSVC, use target_compile_options below
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS ON)

IF(NOT CMAKE_BUILD_TYPE)
  SET(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build (Debug or Release)" FORCE)
ENDIF(NOT CMAKE_BUILD_TYPE)

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} "${CMAKE_SOURCE_DIR}/cmake/modules/")

# find the required packages
find_package(GLM REQUIRED)
message(STATUS "GLM included at ${GLM_INCLUDE_DIR}")
find_package(GLEW REQUIRED)
message(STATUS "Found GLEW in ${GLEW_INCLUDE_DIR}")
find_package(GLFW3 REQUIRED)
message(STATUS "Found GLFW3 in ${GLFW3_INCLUDE_DIR}")
find_package(ASSIMP REQUIRED)
message(STATUS "Found ASSIMP in ${ASSIMP_INCLUDE_DIR}")
find_package(Freetype REQUIRED)
message(STATUS "Found Freetype in ${FREETYPE_INCLUDE_DIRS}")

INCLUDE_DIRECTORIES(/System/Library/Frameworks
  ${CMAKE_CURRENT_LIST_DIR}/includes
)

FIND_LIBRARY(COCOA_LIBRARY Cocoa)
FIND_LIBRARY(OpenGL_LIBRARY OpenGL)
FIND_LIBRARY(IOKit_LIBRARY IOKit)
FIND_LIBRARY(CoreVideo_LIBRARY CoreVideo)
MARK_AS_ADVANCED(COCOA_LIBRARY OpenGL_LIBRARY)
SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
SET(APPLE_LIBS ${APPLE_LIBS} ${GLEW_LIBRARY} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY} ${FREETYPE_LIBRARIES})
set(LIBS ${LIBS} ${APPLE_LIBS})

set(GLFWTESTOSX_SRCs ${CMAKE_CURRENT_LIST_DIR}/main.cpp)
add_executable(glfwTestOSX ${GLFWTESTOSX_SRCs})

set_target_properties(glfwTestOSX PROPERTIES RUNTIME_OUTPUT_DIRECTORY "${CMAKE_SOURCE_DIR}/bin")
set_target_properties(glfwTestOSX PROPERTIES RUNTIME_OUTPUT_DIRECTORY_DEBUG "${CMAKE_SOURCE_DIR}/bin")
set_target_properties(glfwTestOSX PROPERTIES RUNTIME_OUTPUT_DIRECTORY_RELEASE "${CMAKE_SOURCE_DIR}/bin")
target_link_libraries(glfwTestOSX ${LIBS})
