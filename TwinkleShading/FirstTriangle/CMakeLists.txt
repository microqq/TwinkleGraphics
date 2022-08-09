set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(ENABLE_EXPORT_PLUGIN "build ui library" ON)

if(ENABLE_EXPORT_PLUGIN)
    add_definitions(-DEXPORT_PLUGIN)
endif()

set(FirstTriangle_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twFirstTriangle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twFirstTrianglePlugin.cpp
)

add_library(twFirstTriangle SHARED
    ${FirstTriangle_SRCs}
)

set_target_properties(twFirstTriangle PROPERTIES
     LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}/twShading)

target_include_directories(twFirstTriangle PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../Include/Common
    ${CMAKE_CURRENT_LIST_DIR}
)

target_link_libraries(twFirstTriangle twGraphics)