
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
    ${CMAKE_CURRENT_LIST_DIR}/Event/twEventArgs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Event/twEventHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Event/twEventManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Event/twInput.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Event/twInputEventArgs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Util/twGeoUtil.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Util/twComUtil.cpp
)

set(TWEditorLib_Srcs
    ${CMAKE_CURRENT_LIST_DIR}/Entry/twApplication.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twHierarchy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twInspector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twEditorMenu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twEditorWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twAssetsWindow.cpp
)

set(TWUILib_Srcs
    ${CMAKE_CURRENT_LIST_DIR}/UI/twUiUtil.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twMainFrame.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twMainWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twSizePolicy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twDockSpaceLayout.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twDockingWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/GLFW/twGLFWMainFrame.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/GLFW/twGLFWViewWindow.cpp
)

add_library(twGraphics SHARED ${TWGraphics_SRCs})
add_library(twEditorLib SHARED ${TWEditorLib_Srcs})
add_library(twUILib SHARED ${TWUILib_Srcs})

set_target_properties(twGraphics PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})
set_target_properties(twEditorLib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})
set_target_properties(twUILib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

target_include_directories(twGraphics PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Common
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Event
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Util
)

target_include_directories(twEditorLib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Editor
)

target_include_directories(twUILib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/imgui
    ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/imgui/backends
    ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/imgui/misc/freetype
    ${CMAKE_CURRENT_LIST_DIR}/../Include/UI
    ${CMAKE_CURRENT_LIST_DIR}/../Include/UI/GLFW
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Entry
)

if(ENABLE_DEBUG)
    target_compile_options(twGraphics PRIVATE -Wall -Wno-extra)
    target_compile_options(twEditorLib PRIVATE -Wall -Wno-extra)
    target_compile_options(twUILib PRIVATE -Wall -Wno-extra)
endif()

if(LINUX)
    set(LINUX_LIBS ${GLEW_LIBRARY} ${GLFW3_LIBRARY} ${ASSIMP_LIBRARY} ${FREEIMAGE_LIBRARY}
        imgui
        imGuiFileDialog
        vermilion
    )
    set(LINUX_LIBS ${LINUX_LIBS} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread)
    set(TWGRAPHICS_DEPEND_LIBS ${LINUX_LIBS})
elseif(APPLE)
    SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
    SET(APPLE_LIBS 
        ${APPLE_LIBS}  
        # ${GLEW_LIBRARY} 
        # ${GLFW3_LIBRARY} 
        ${ASSIMP_LIBRARY} 
        ${FREEIMAGE_LIBRARY}
        imgui
        imGuiFileDialog
        vermilion
    )
    set(TWGRAPHICS_DEPEND_LIBS ${APPLE_LIBS})
elseif(WIN32)
    set(WIN32_LIBS opengl32 assimp freeimage imgui imGuiFileDialog vermilion)
    set(TWGRAPHICS_DEPEND_LIBS ${WIN32_LIBS})
else()
    set(TWGRAPHICS_DEPEND_LIBS )
endif(LINUX)

# set twGraphics link libraries
target_link_libraries(twGraphics ${TWGRAPHICS_DEPEND_LIBS})
target_link_libraries(twUILib twGraphics)
target_link_libraries(twEditorLib twUILib)

# twEditor
add_executable(twEditor ${CMAKE_CURRENT_LIST_DIR}/Editor/twEditor.cpp)
target_link_libraries(twEditor twEditorLib)