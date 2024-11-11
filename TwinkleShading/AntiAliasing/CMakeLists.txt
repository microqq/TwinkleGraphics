
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(AntiAliasing_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twAntiAliasing.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twAntiAliasingPlugin.cpp
)

add_library(twAntiAliasing SHARED
    ${AntiAliasing_SRCs}
)
set_target_properties(twAntiAliasing PROPERTIES
     LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}/twShading)

target_include_directories(twAntiAliasing PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
)

if(ENABLE_DEBUG)
    target_compile_options(twAntiAliasing PRIVATE -Wall -Wno-extra)
endif()

target_link_libraries(twAntiAliasing glew32 
    imgui twGraphics twUILib twShadingView twImGuiContextMgr twUiUtil)