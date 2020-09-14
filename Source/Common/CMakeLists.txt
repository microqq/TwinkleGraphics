
set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(DEFINE_FREEIMAGE_LIB "use static freeimage lib" OFF)
OPTION(ENABLE_EXPORT_COMDLL "build common dynamic library" ON)
if(DEFINE_FREEIMAGE_LIB)
    add_definitions(-DFREEIMAGE_LIB)
endif()

if(ENABLE_EXPORT_COMDLL)
    add_definitions(-DEXPORT_COMDLL)
endif()


set(Common_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twCommon.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twMainWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twDynLib.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twPluginManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twView.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twResource.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twResourceManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twMeshRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twRenderContext.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twRenderPipeline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twForwardPipeline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twDeferredPipeline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twTextReader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twTextManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twShader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twShaderManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twMesh.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twGeometry.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twModel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twModelManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twModelReader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twTransform.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twCamera.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twOrbitControl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twFirstPersonControl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twImage.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twImageManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twTexture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twTextureManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twSprite.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twUniform.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twMaterial.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twSceneManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twRenderTexture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twHWBuffers.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twConsoleLog.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twRingBuffer.cpp
)

add_library(twCommon SHARED ${Common_SRCs})
target_include_directories(twCommon PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../../ThirdParty
    ${CMAKE_CURRENT_LIST_DIR}/../../ThirdParty/imgui
    ${CMAKE_CURRENT_LIST_DIR}/../../ThirdParty/FreeImage
    ${CMAKE_CURRENT_LIST_DIR}/../../ThirdParty/vermilion
    ${CMAKE_CURRENT_LIST_DIR}/../../Include/Common
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set_target_properties(twCommon PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "MacOS")
    set_target_properties(twCommon PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)
endif()

# target_compile_definitions(twCommon PUBLIC FREEIMAGE_LIB=1)

# #CMake Error: Cannot determine link language for target "twCommon"
# set_target_properties(twCommon PROPERTIES LINKER_LANGUAGE CXX)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(twCommon -lglfw -lGL -lGLEW imgui vermilion)
    target_link_libraries(twCommon ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/FreeImage/linux/libfreeimage-3.18.0.so)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "MacOS")
    target_link_libraries(twCommon -lglfw -lGLEW -lfreeimage -lassimp imgui vermilion)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(twCommon OpenGL32.lib)
    target_link_libraries(twCommon ${GLEWLIBDIR}/glew32.lib)
    target_link_libraries(twCommon ${GLFWLIBDIR}/libglfw3dll.a)
    target_link_libraries(twCommon ${PROJECT_SOURCE_DIR}/ThirdParty/assimp/libs/mingw-w64/libassimp.dll.a)
    target_link_libraries(twCommon ${PROJECT_SOURCE_DIR}/ThirdParty/FreeImage/win/FreeImage.lib)
    target_link_libraries(twCommon imgui)
    target_link_libraries(twCommon vermilion)
endif()