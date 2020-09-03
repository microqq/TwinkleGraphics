
set(CMAKE_INCLUDE_CURRENT_DIR ON)

option(ENABLE_EXPORT_EVENT "build singleton dynamic library" ON)

if(ENABLE_EXPORT_EVENT)
    add_definitions(-DEXPORT_EVENT)
endif()

set(EVENT_SRC
    ${CMAKE_CURRENT_LIST_DIR}/twEventArgs.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twEventHandler.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twEventManager.cpp
)

add_library(twEvent SHARED ${EVENT_SRC})
target_include_directories(twEvent PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../../Include/Common
    ${CMAKE_CURRENT_LIST_DIR}/../../Include/Event
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set_target_properties(twEvent PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(twEvent twCommon)
endif()
