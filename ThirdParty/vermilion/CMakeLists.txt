set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(VERMILION_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/vdds.cpp
)

# add vermilion library
add_library(vermilion SHARED ${VERMILION_SRCs})

set_target_properties(vermilion PROPERTIES
    LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH})

# set include directory
target_include_directories(vermilion PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
)
target_compile_definitions(vermilion PUBLIC _DEBUG=1)
target_compile_options(vermilion PUBLIC -fno-elide-constructors)