
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(IMGUI_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/imgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui_draw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui_widgets.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui_demo.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/imgui_filebrowser.h
)

add_library(imgui SHARED
    ${IMGUI_SRCs}
)
# set_property(TARGET imgui PROPERTY CXX_STANDARD 17)
target_include_directories(imgui PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
)
# target_compile_features(imgui PRIVATE cxx_std_17)
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW=1)
# target_link_libraries(imgui -lstdc++fs)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(imgui OpenGL32.lib)
    target_link_libraries(imgui ${GLEWLIBDIR}/glew32.lib)
    target_link_libraries(imgui ${GLFWLIBDIR}/libglfw3dll.a)
endif()
