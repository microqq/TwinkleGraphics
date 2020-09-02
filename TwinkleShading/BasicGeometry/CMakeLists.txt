

set(CMAKE_INCLUDE_CURRENT_DIR ON)

OPTION(ENABLE_BUILDING_DLL "build dynamic library" ON)
IF(ENABLE_BUILDING_DLL)
    add_definitions(-DBUILDING_DLL)
ENDIF(ENABLE_BUILDING_DLL)

set(BasicGeometry_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twBasicGeometry.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twBasicGeometryPlugin.cpp
)

add_library(twBasicGeometry SHARED
    ${BasicGeometry_SRCs}
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set_target_properties(twBasicGeometry PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs/twShading)
endif()

target_include_directories(twBasicGeometry PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../../ThirdParty
    ${CMAKE_CURRENT_LIST_DIR}/../../Common
    ${CMAKE_CURRENT_LIST_DIR}
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(twBasicGeometry twCommon imgui twUtil)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(twBasicGeometry OpenGL32.lib)
    target_link_libraries(twBasicGeometry ${GLEWLIBDIR}/glew32.lib)
    target_link_libraries(twBasicGeometry ${GLFWLIBDIR}/libglfw3dll.a)
    target_link_libraries(twBasicGeometry twCommon)
    target_link_libraries(twBasicGeometry twUtil)
    target_link_libraries(twBasicGeometry imgui)
endif()

# if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
#     if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwBasicGeometry.so)
#         file(COPY 
#             ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwBasicGeometry.so 
#             DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/twShading/
#         )
#         file(REMOVE
#             ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwBasicGeometry.so
#         )
#     endif()
# endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwBasicGeometry.dll)
        file(COPY 
            ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwBasicGeometry.dll 
            DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/twShading/
        )
        file(REMOVE
            ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwBasicGeometry.dll
        )
    endif()
endif()