
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

target_include_directories(imgui PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
)

target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW=1)

# if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
if(LINUX)
    set_target_properties(imgui PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)
endif()

if(APPLE)
    message(STATUS "Beging build imgui(Platform: MacOS)......")

    set_target_properties(imgui PROPERTIES
        LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)

    target_link_libraries(imgui libGLEW.dylib libGLFW.3.dylib
        "-framework OpenGL"
        "-framework IOKit"
        "-framework CoreVideo"
        "-framework Cocoa"
    )        
endif()

# if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
if(WIN32)
    target_link_libraries(imgui OpenGL32.lib)
    target_link_libraries(imgui ${GLEWLIBDIR}/glew32.lib)
    target_link_libraries(imgui ${GLFWLIBDIR}/libglfw3dll.a)
endif()
