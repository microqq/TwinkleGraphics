set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(FirstTriangle_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twFirstTriangle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twFirstTrianglePlugin.cpp
)

add_library(twFirstTriangle SHARED
    ${FirstTriangle_SRCs}
)

set_target_properties(twFirstTriangle PROPERTIES
     LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}/twShading)

target_include_directories(twFirstTriangle PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(twFirstTriangle glew32 imgui::imgui twGraphics twShadingView)