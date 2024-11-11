
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(TWGraphics_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/Common/twCommon.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twDynLib.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Common/twPluginManager.cpp
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

set(TWUILib_Srcs
    ${CMAKE_CURRENT_LIST_DIR}/UI/twMainFrame.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/UI/twSizePolicy.cpp
)

set(TWEditor_Srcs
    ${CMAKE_CURRENT_LIST_DIR}/Entry/twApplication.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twHierarchy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twInspector.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twEditorMenu.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twEditorWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twAssetsWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twEditor.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/twUiUtil.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/GLFW/twGLFWMainFrame.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/GLFW/twGLFWViewWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/ImGui/twImGuiContextManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/ImGuiDocking/twDockingWidget.cpp
    ${CMAKE_CURRENT_LIST_DIR}/Editor/ImGuiDocking/twDockSpaceLayout.cpp
)

add_library(twGraphics SHARED ${TWGraphics_SRCs})
add_library(twUILib SHARED ${TWUILib_Srcs})
add_executable(twEditor ${TWEditor_Srcs})

set_target_properties(twGraphics PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})
set_target_properties(twUILib PROPERTIES LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

target_include_directories(twGraphics PUBLIC 
    ${PROJECT_SOURCE_DIR}/build/vcpkg_installed/x64-mingw-dynamic-openexr/include
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Common
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Event
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Util
    ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/vermilion
)

target_include_directories(twUILib PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../Include/UI
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Entry
)

target_include_directories(twEditor PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Editor
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Editor/GLFW
)

if(ENABLE_DEBUG)
    target_compile_options(twGraphics PRIVATE -Wall)
    target_compile_options(twUILib PRIVATE -Wall)
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
    set(WIN32_LIBS opengl32 GLEW::GLEW freeimage::FreeImage vermilion assimp::assimp)
    set(TWGRAPHICS_DEPEND_LIBS ${WIN32_LIBS})
else()
    set(TWGRAPHICS_DEPEND_LIBS )
endif(LINUX)

# set twGraphics link libraries
target_link_libraries(twGraphics PUBLIC ${TWGRAPHICS_DEPEND_LIBS})
target_link_libraries(twUILib PUBLIC glfw imgui::imgui imGuiFileDialog twGraphics)

# twEditor
target_link_libraries(twEditor PUBLIC twUILib)