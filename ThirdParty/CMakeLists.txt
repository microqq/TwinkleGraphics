
set(CMAKE_INCLUDE_CURRENT_DIR ON)

#################### ImGuiFIleDialog ##########################

set(IMGUIFILEDIALOG_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/ImGuiFileDialog/ImGuiFileDialog.cpp
)

if(APPLE)
    add_library(imGuiFileDialog SHARED ${IMGUIFILEDIALOG_SRCs})
else()
    add_library(imGuiFileDialog STATIC ${IMGUIFILEDIALOG_SRCs})
endif(APPLE)

set_target_properties(imGuiFileDialog PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

# set include directory
target_include_directories(imGuiFileDialog PUBLIC 
    ${PROJECT_SOURCE_DIR}/build/vcpkg_installed/x64-mingw-dynamic-openexr/include
    ${CMAKE_CURRENT_LIST_DIR}/ImGuiFileDialog
    ${CMAKE_CURRENT_LIST_DIR}/ImGuiFileDialog/stb
)

if(WIN32)
    target_include_directories(imGuiFileDialog PUBLIC 
        ${CMAKE_CURRENT_LIST_DIR}/ImGuiFIleDialog/dirent)
endif(WIN32)

# set compile definitions
target_compile_definitions(imGuiFileDialog PUBLIC USE_STD_FILESYSTEM=1)

# set link libraries
target_link_libraries(imGuiFileDialog PUBLIC imgui)


#################### vermilion ##########################

set(VERMILION_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/vermilion/vdds.cpp
)

# add vermilion library
add_library(vermilion STATIC ${VERMILION_SRCs})

set_target_properties(vermilion PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

# set include directory
target_include_directories(vermilion PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/vermilion
)
target_compile_definitions(vermilion PUBLIC _DEBUG=1)
target_compile_options(vermilion PUBLIC -fno-elide-constructors)