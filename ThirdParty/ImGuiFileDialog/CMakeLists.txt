
set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(IMGUIFILEDIALOG_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/ImguiFileDialog.cpp
)

add_library(imguiFileDialog SHARED
    ${IMGUIFILEDIALOG_SRCs}
)

target_include_directories(imguiFileDialog PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
)

# target_compile_features(imgui PRIVATE cxx_std_17)
# target_compile_definitions(imguiFileDialog PUBLIC IMGUI_IMPL_OPENGL_LOADER_GLEW=1)
# target_link_libraries(imgui -lstdc++fs)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set_target_properties(imguiFileDialog PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "MacOS")
    set_target_properties(imguiFileDialog PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)

    target_link_libraries(imguiFileDialog imgui)         
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(imguiFileDialog OpenGL32.lib)
    target_link_libraries(imguiFileDialog ${GLEWLIBDIR}/glew32.lib)
    target_link_libraries(imguiFileDialog ${GLFWLIBDIR}/libglfw3dll.a)
    target_link_libraries(imguiFileDialog imgui)
endif()
