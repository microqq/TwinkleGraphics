
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(IMGUIFILEDIALOG_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/ImguiFileDialog.cpp
)

add_library(imguiFileDialog SHARED ${IMGUIFILEDIALOG_SRCs})

set_target_properties(imguiFileDialog PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

# set include directory
target_include_directories(imguiFileDialog PUBLIC ${CMAKE_CURRENT_LIST_DIR})

if(WIN32)
    target_include_directories(imguiFileDialog PUBLIC ${DIRENTINCLUDEDIR})
endif(WIN32)

# set link libraries
target_link_libraries(imguiFileDialog imgui)
