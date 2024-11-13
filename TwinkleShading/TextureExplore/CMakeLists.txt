set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(TextureExplore_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twTextureExplore.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twTextureExplorePlugin.cpp
)

add_library(twTextureExplore SHARED
    ${TextureExplore_SRCs}
)

set_target_properties(twTextureExplore PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}/twShading)

target_include_directories(twTextureExplore PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(twTextureExplore glew32 
    imgui twGraphics twShadingView twImGuiContextMgr)
