
set(CMAKE_INCLUDE_CURRENT_DIR ON)

OPTION(DEFINE_FREEIMAGE_LIB "use static freeimage lib" OFF)
IF(DEFINE_FREEIMAGE_LIB)
    add_definitions(FREEIMAGE_LIB)
ENDIF(DEFINE_FREEIMAGE_LIB)


SET(Common_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twMainWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twDynLib.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twPluginManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twView.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twResource.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twMeshRenderer.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twRenderContext.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twRenderPipeline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twForwardPipeline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twDeferredPipeline.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twShader.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twMesh.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twGeometry.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twTransform.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twCamera.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twOrbitControl.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twImage.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twTexture.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twSprite.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twMaterial.cpp
)

SET(Util_SRC
    ${CMAKE_CURRENT_LIST_DIR}/twUtil.cpp
)

add_library(twCommon SHARED ${Common_SRCs})
target_include_directories(twCommon PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty
    ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/imgui
    ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/FreeImage
    ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/vermilion
    ${CMAKE_CURRENT_LIST_DIR}
)
target_compile_definitions(twCommon PUBLIC _DEBUG=1)
target_compile_definitions(twCommon PUBLIC TEMPORARY_USE=1)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set_target_properties(twCommon PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)
endif()

add_library(twUtil SHARED ${Util_SRC})
target_include_directories(twUtil PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}
)

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set_target_properties(twUtil PROPERTIES
         LIBRARY_OUTPUT_DIRECTORY ${PROJECT_SOURCE_DIR}/Output/libs)
endif()

# target_compile_definitions(twCommon PUBLIC FREEIMAGE_LIB=1)

# #CMake Error: Cannot determine link language for target "twCommon"
# set_target_properties(twCommon PROPERTIES LINKER_LANGUAGE CXX)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(twCommon -lglfw -lGL -lGLEW imgui vermilion)
    target_link_libraries(twCommon ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/FreeImage/linux/libfreeimage-3.18.0.so)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(twCommon OpenGL32.lib)
    target_link_libraries(twCommon ${GLEWLIBDIR}/glew32.lib)
    target_link_libraries(twCommon ${GLFWLIBDIR}/libglfw3dll.a)
    target_link_libraries(twCommon ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty/FreeImage/win/FreeImage.dll)
    target_link_libraries(twCommon imgui)
    target_link_libraries(twCommon vermilion)
    target_link_libraries(twUtil twCommon)
endif()
