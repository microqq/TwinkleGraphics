set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(VERMILION_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/vdds.cpp
)

add_library(vermilion SHARED
    ${VERMILION_SRCs}
)
target_include_directories(vermilion PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
)
target_compile_definitions(vermilion PUBLIC _DEBUG=1)
target_compile_options(vermilion PUBLIC -fno-elide-constructors)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set_target_properties(vermilion PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "MacOS")
    set_target_properties(vermilion PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)
endif()

# if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
#     target_link_libraries(imgui OpenGL32.lib)
#     target_link_libraries(imgui ${GLEWLIBDIR}/glew32.lib)
#     target_link_libraries(imgui ${GLFWLIBDIR}/libglfw3dll.a)
# endif()
