set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(BasicGeometry_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twBasicGeometry.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twBasicGeometryPlugin.cpp
)

add_library(twBasicGeometry SHARED
    ${BasicGeometry_SRCs}
)

set_target_properties(twBasicGeometry PROPERTIES
     LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}/twShading)

target_include_directories(twBasicGeometry PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(twBasicGeometry glew32 imgui::imgui 
    twGraphics twShadingView twImGuiContextMgr)