
set(TwinkleShading_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twView.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twMainWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twShading.cpp
)

set(ContextMgr_SRCs
    ${CMAKE_CURRENT_LIST_DIR}/../Source/Editor/ImGui/twImGuiContextManager.cpp
)

set(UIUtil_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/../Source/Editor/twUiUtil.cpp
)

set(ShadingView_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twView.cpp
)

add_library(twImGuiContextMgr SHARED ${ContextMgr_SRCs})
target_link_libraries(twImGuiContextMgr imgui::imgui twGraphics)

add_library(twUiUtil SHARED ${UIUtil_SRCs})
target_link_libraries(twUiUtil imGuiFileDialog twGraphics twImGuiContextMgr)

add_library(twShadingView SHARED ${ShadingView_SRCs})
target_link_libraries(twShadingView twGraphics)

add_subdirectory(FirstTriangle)
add_subdirectory(BasicGeometry)
add_subdirectory(TextureExplore)
add_subdirectory(AntiAliasing)

## add executeable 
add_executable(twShading ${TwinkleShading_SRCs})
target_link_libraries(twShading glew32 glfw imgui::imgui 
    twGraphics twUILib twImGuiContextMgr)