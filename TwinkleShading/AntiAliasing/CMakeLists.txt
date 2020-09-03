

set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(AntiAliasing_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twAntiAliasing.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twAntiAliasingPlugin.cpp
)

add_library(twAntiAliasing SHARED
    ${AntiAliasing_SRCs}
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set_target_properties(twAntiAliasing PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs/twShading)
endif()

target_include_directories(twAntiAliasing PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../../ThirdParty
    ${CMAKE_CURRENT_LIST_DIR}/../../ThirdParty/ImGuiFileDialog
    ${CMAKE_CURRENT_LIST_DIR}/../../Include/Common
    ${CMAKE_CURRENT_LIST_DIR}/../../Include/UI
    ${CMAKE_CURRENT_LIST_DIR}/../../Include/Event
    ${CMAKE_CURRENT_LIST_DIR}
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(twAntiAliasing twCommon imgui twUtil)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(twAntiAliasing OpenGL32.lib)
    target_link_libraries(twAntiAliasing ${GLEWLIBDIR}/glew32.lib)
    target_link_libraries(twAntiAliasing ${GLFWLIBDIR}/libglfw3dll.a)
    target_link_libraries(twAntiAliasing twCommon)
    target_link_libraries(twAntiAliasing twUtil)
    target_link_libraries(twAntiAliasing imgui)
    target_link_libraries(twAntiAliasing twUiUtil)
endif()

# if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
#     if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwAntiAliasing.so)
#         file(COPY 
#             ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwAntiAliasing.so 
#             DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/twShading/
#         )
#         file(REMOVE
#             ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwAntiAliasing.so
#         )
#     endif()
# endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwAntiAliasing.dll)
        file(COPY 
            ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwAntiAliasing.dll 
            DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/twShading/
        )
        file(REMOVE
            ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwAntiAliasing.dll
        )
    endif()
endif()