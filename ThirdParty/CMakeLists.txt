
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#################### IMGUI ##########################

set(IMGUI_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui/backends/imgui_impl_glfw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui/backends/imgui_impl_opengl3.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_draw.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_widgets.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_tables.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui/misc/freetype/imgui_freetype.cpp
    ${CMAKE_CURRENT_LIST_DIR}/imgui/imgui_demo.cpp
    # ${CMAKE_CURRENT_LIST_DIR}/imgui_filebrowser.h
)

# add library
add_library(imgui SHARED ${IMGUI_SRCs})

# set output directory
set_target_properties(imgui PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

# set include directory
target_include_directories(imgui PUBLIC 
    ${FREETYPE_INCLUDE_DIR}
    ${CMAKE_CURRENT_LIST_DIR}/imgui
    ${CMAKE_CURRENT_LIST_DIR}/imgui/misc/freetype
)

# set opengl loader
target_compile_definitions(imgui PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW=1)
# target_compile_definitions(imgui PUBLIC IMGUI_ENABLE_FREETYPE=1)

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
    set(IMGUI_DEPEND_LIBS glew32 glfw3 opengl32)
    add_definitions(-D_CRT_SECURE_NO_WARNINGS)
else()
    set(IMGUI_DEPEND_LIBS )
endif(LINUX)

target_link_libraries(imgui ${IMGUI_DEPEND_LIBS} ${FREETYPE_LIBRARIES})


#################### ImGuiFIleDialog ##########################

set(IMGUIFILEDIALOG_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/ImGuiFileDialog/ImGuiFileDialog.cpp
)

add_library(imGuiFileDialog SHARED ${IMGUIFILEDIALOG_SRCs})

set_target_properties(imGuiFileDialog PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

# set include directory
target_include_directories(imGuiFileDialog PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/ImGuiFileDialog
    ${CMAKE_CURRENT_LIST_DIR}/ImGuiFileDialog/stb
)

if(WIN32)
    target_include_directories(imGuiFileDialog PUBLIC ${DIRENTINCLUDEDIR})
endif(WIN32)

# set link libraries
target_link_libraries(imGuiFileDialog imgui)


#################### vermilion ##########################

set(VERMILION_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/vermilion/vdds.cpp
)

# add vermilion library
add_library(vermilion SHARED ${VERMILION_SRCs})

set_target_properties(vermilion PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

# set include directory
target_include_directories(vermilion PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/vermilion
)
target_compile_definitions(vermilion PUBLIC _DEBUG=1)
target_compile_options(vermilion PUBLIC -fno-elide-constructors)