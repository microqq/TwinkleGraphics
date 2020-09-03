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

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set_target_properties(twFirstTriangle PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs/twShading)
endif()

target_include_directories(twFirstTriangle PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../../ThirdParty
    ${CMAKE_CURRENT_LIST_DIR}/../../Common
    ${CMAKE_CURRENT_LIST_DIR}
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(twFirstTriangle twCommon imgui twUtil)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(twFirstTriangle OpenGL32.lib)
    target_link_libraries(twFirstTriangle ${GLEWLIBDIR}/glew32.lib)
    target_link_libraries(twFirstTriangle ${GLFWLIBDIR}/libglfw3dll.a)
    target_link_libraries(twFirstTriangle twCommon)
    target_link_libraries(twFirstTriangle twUtil)
    target_link_libraries(twFirstTriangle imgui)
endif()

# if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
#     if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwFirstTriangle.so)
#         file(COPY 
#             ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwFirstTriangle.so 
#             DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/twShading/
#         )
#         file(REMOVE
#             ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwFirstTriangle.so
#         )
#     endif()
# endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwFirstTriangle.dll)
        file(COPY 
            ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwFirstTriangle.dll 
            DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/twShading/
        )
        file(REMOVE
            ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwFirstTriangle.dll
        )
    endif()
endif()