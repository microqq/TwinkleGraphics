
set(CMAKE_INCLUDE_CURRENT_DIR ON)

SET(Common_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twMainWindow.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twDynLib.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twPluginManager.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twView.cpp
)

add_library(twCommon SHARED ${Common_SRCs})
target_include_directories(twCommon PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty
    ${CMAKE_CURRENT_LIST_DIR}
)
# #CMake Error: Cannot determine link language for target "twCommon"
# set_target_properties(twCommon PROPERTIES LINKER_LANGUAGE CXX)
if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(twCommon -lglfw -lGL -lGLEW)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(twCommon OpenGL32.lib)
    target_link_libraries(twCommon ${GLEWLIBDIR}/glew32.lib)
    target_link_libraries(twCommon ${GLFWLIBDIR}/libglfw3dll.a)
endif()
