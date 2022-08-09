
set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(ENABLE_EXPORT_PLUGIN "build ui library" ON)

if(ENABLE_EXPORT_PLUGIN)
    add_definitions(-DEXPORT_PLUGIN)
endif()

set(AntiAliasing_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twAntiAliasing.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twAntiAliasingPlugin.cpp
)

add_library(twAntiAliasing SHARED
    ${AntiAliasing_SRCs}
)
set_target_properties(twAntiAliasing PROPERTIES
     LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}/twShading)

target_include_directories(twAntiAliasing PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../../Include/Common
    ${CMAKE_CURRENT_LIST_DIR}/../../Include/UI
    ${CMAKE_CURRENT_LIST_DIR}/../../Include/Event
    ${CMAKE_CURRENT_LIST_DIR}
)

if(ENABLE_DEBUG)
    target_compile_options(twAntiAliasing PRIVATE -Wall -Wno-extra)
endif()

target_link_libraries(twAntiAliasing twGraphics)