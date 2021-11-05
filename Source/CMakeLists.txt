
set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(DEFINE_FREEIMAGE_LIB "use static freeimage lib" OFF)
option(ENABLE_EXPORT_COMDLL "build common dynamic library" ON)

if(DEFINE_FREEIMAGE_LIB)
    add_definitions(-DFREEIMAGE_LIB)
endif()

if(ENABLE_EXPORT_COMDLL)
    add_definitions(-DEXPORT_COMDLL)
endif()

set(TWGraphics_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/Common/twCommon.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twDynLib.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twPluginManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twView.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twResource.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twResourceManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twMeshRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twRenderContext.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twRenderPipeline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twForwardPipeline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twDeferredPipeline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twTextReader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twTextManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twShader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twShaderManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twLight.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twMesh.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twGeometry.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twModel.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twModelManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twModelReader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twTransform.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twCamera.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twOrbitControl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twFirstPersonControl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twImage.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twImageManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twTexture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twTextureManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twSprite.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twUniform.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twMaterial.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twSceneManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twSceneNode.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twRenderTexture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twHWBuffers.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twConsoleLog.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twBoundingVolume.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twFrustum.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Entry/twApplication.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Event/twEventArgs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Event/twEventHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Event/twEventManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Event/twInput.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Event/twInputEventArgs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twUiUtil.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twMainFrame.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twMainWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twSizePolicy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/GLFW/twGLFWMainFrame.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/GLFW/twGLFWViewWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twHierarchy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twInspector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twEditorMenu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twEditorWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Util/twGeoUtil.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Util/twComUtil.cpp
)

add_library(twGraphics SHARED ${TWGraphics_SRCs})
target_include_directories(twGraphics PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty
    ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/imgui
    ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/ImGuiFileDialog
    ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/FreeImage
    ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/vermilion
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Common
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Entry
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Event
    ${CMAKE_CURRENT_LIST_DIR}/../Include/UI
    ${CMAKE_CURRENT_LIST_DIR}/../Include/UI/GLFW
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Editor
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Util
)
if(ENABLE_DEBUG)
    target_compile_options(twGraphics PRIVATE -Wall -Wno-extra)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set_target_properties(twGraphics PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)
    target_link_libraries(twGraphics -lglfw -lGL -lGLEW imgui vermilion)
    target_link_libraries(twGraphics ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/FreeImage/linux/libfreeimage-3.18.0.so)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(twGraphics OpenGL32.lib)
    target_link_libraries(twGraphics ${GLEWLIBDIR}/glew32.lib)
    target_link_libraries(twGraphics ${GLFWLIBDIR}/libglfw3dll.a)
    target_link_libraries(twGraphics ${PROJECT_SOURCE_DIR}/ThirdParty/assimp/libs/mingw-w64/libassimp.dll.a)
    target_link_libraries(twGraphics ${PROJECT_SOURCE_DIR}/ThirdParty/FreeImage/win/FreeImage.lib)
    target_link_libraries(twGraphics imgui)
    target_link_libraries(twGraphics imguiFileDialog)    
    target_link_libraries(twGraphics vermilion)
endif()


add_executable(twEditor ${CMAKE_CURRENT_LIST_DIR}/Entry/twEditor.cpp)
target_include_directories(twEditor PUBLIC ${CMAKE_CURRENT_LIST_DIR}/Include/Entry)
target_link_libraries(twEditor twGraphics)