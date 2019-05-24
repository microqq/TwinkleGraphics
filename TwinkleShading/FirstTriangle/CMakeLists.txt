

set(CMAKE_INCLUDE_CURRENT_DIR ON)

set(FirstTriangle_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twFirstTriangle.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twFirstTrianglePlugin.cpp
)

add_library(twFirstTriangle SHARED
    ${FirstTriangle_SRCs}
)

target_include_directories(twFirstTriangle PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../../ThirdParty
    ${CMAKE_CURRENT_LIST_DIR}/../../Common
    ${CMAKE_CURRENT_LIST_DIR}
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(twFirstTriangle twCommon)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(twFirstTriangle OpenGL32.lib)
    target_link_libraries(twFirstTriangle "E:/Codes/glew-2.1.0/lib/Release/x64/glew32.lib")
    target_link_libraries(twFirstTriangle "E:/Codes/glfw-3.3.bin.WIN64/lib-mingw-w64/libglfw3dll.a")
    target_link_libraries(twFirstTriangle twCommon)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    if(EXISTS ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwFirstTriangle.so)
        file(COPY 
            ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwFirstTriangle.so 
            DESTINATION ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/twShading/
        )
        file(REMOVE
            ${CMAKE_CURRENT_LIST_DIR}/../../Output/libs/libtwFirstTriangle.so
        )
    endif()
endif()

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