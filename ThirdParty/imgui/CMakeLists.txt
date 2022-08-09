
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

# add library
add_library(imgui SHARED ${IMGUI_SRCs})

# set output directory
set_target_properties(imgui PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

# set include directory
target_include_directories(imgui PUBLIC ${CMAKE_CURRENT_LIST_DIR})

# set opengl loader
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW=1)

if(LINUX)
    set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall")
    find_package(OpenGL REQUIRED)
    add_definitions(${OPENGL_DEFINITIONS})
    find_package(X11 REQUIRED)
    # note that the order is important for setting the libs
    # use pkg-config --libs $(pkg-config --print-requires --print-requires-private glfw3) in a terminal to confirm
    set(IMGUI_DEPEND_LIBS ${GLFW3_LIBRARY} X11 Xrandr Xinerama Xi Xxf86vm Xcursor GL dl pthread freetype ${ASSIMP_LIBRARY})
    set (CMAKE_CXX_LINK_EXECUTABLE "${CMAKE_CXX_LINK_EXECUTABLE} -ldl")
elseif(APPLE)
    SET(APPLE_LIBS ${COCOA_LIBRARY} ${IOKit_LIBRARY} ${OpenGL_LIBRARY} ${CoreVideo_LIBRARY})
    SET(APPLE_LIBS ${APPLE_LIBS}  ${GLEW_LIBRARY} ${GLFW3_LIBRARY})
    set(IMGUI_DEPEND_LIBS ${APPLE_LIBS})
elseif(WIN32)
    set(IMGUI_DEPEND_LIBS glew glfw3 opengl32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    set(IMGUI_DEPEND_LIBS )
endif(LINUX)

target_link_libraries(imgui ${IMGUI_DEPEND_LIBS})
