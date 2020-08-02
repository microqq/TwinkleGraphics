
set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(UiUtil_SRC
    ${CMAKE_CURRENT_LIST_DIR}/twUiUtil.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twHierarchy.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twInspector.cpp
)

add_library(twUiUtil SHARED ${UiUtil_SRC})
target_include_directories(twUiUtil PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../../ThirdParty/imgui
    ${CMAKE_CURRENT_LIST_DIR}/../../Include/Common
    ${CMAKE_CURRENT_LIST_DIR}/../../Include/UI
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set_target_properties(twUiUtil PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(twUiUtil imguiFileDialog)
endif()
