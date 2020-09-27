set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(ENABLE_EXPORT_UTILDLL "build ui library" ON)

if(ENABLE_EXPORT_UTILDLL)
    add_definitions(-DEXPORT_UTILDLL)
endif()


SET(Util_SRC
    ${CMAKE_CURRENT_LIST_DIR}/twUtil.cpp
)

add_library(twUtil SHARED ${Util_SRC})
target_include_directories(twUtil PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
)

if(ENABLE_DEBUG)
    target_compile_options(twUtil PRIVATE "-Wall")
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set_target_properties(twUtil PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "MacOS")
    set_target_properties(twUtil PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)
endif()

target_link_libraries(twUtil twCommon)