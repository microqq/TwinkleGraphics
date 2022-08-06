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

if(LINUX)
    set_target_properties(vermilion PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)
endif()

if(WIN32)
    set_target_properties(vermilion PROPERTIES
        RUNTIME_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)
endif()