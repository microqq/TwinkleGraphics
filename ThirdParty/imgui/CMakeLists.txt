
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(IMGUI_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/imgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui_draw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui_widgets.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui_demo.cpp
)

add_library(imgui SHARED
    ${IMGUI_SRCs}
)

target_include_directories(imgui PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW=1)
