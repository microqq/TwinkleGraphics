

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(TextureExplore_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twTextureExplore.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twTextureExplorePlugin.cpp
)

add_library(twTextureExplore SHARED
    ${TextureExplore_SRCs}
)

set_target_properties(twTextureExplore PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${LIBRARY_OUTPUT_PATH}/twShading)

target_include_directories(twTextureExplore PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../../ThirdParty
    ${CMAKE_CURRENT_LIST_DIR}/../../Common
    ${CMAKE_CURRENT_LIST_DIR}
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(twTextureExplore twCommon imgui)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(twTextureExplore OpenGL32.lib)
    target_link_libraries(twTextureExplore ${GLEWLIBDIR}/glew32.lib)
    target_link_libraries(twTextureExplore ${GLFWLIBDIR}/libglfw3dll.a)
    target_link_libraries(twTextureExplore twCommon)
    target_link_libraries(twTextureExplore twUtil)
    target_link_libraries(twTextureExplore imgui)
endif()

# if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
#     if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwTextureExplore.so)
#         file(COPY 
#             ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwTextureExplore.so 
#             DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/twShading/
#         )
#         file(REMOVE
#             ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwTextureExplore.so
#         )
#     endif()
# endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwTextureExplore.dll)
        file(COPY 
            ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwTextureExplore.dll 
            DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/twShading/
        )
        file(REMOVE
            ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwTextureExplore.dll
        )
    endif()
endif()