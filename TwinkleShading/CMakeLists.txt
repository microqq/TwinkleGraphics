
set(TwinkleShading_SRCs 
    ${CMAKE_CURRENT_LIST_DIR}/twShading.cpp
)

add_subdirectory(FirstTriangle)
add_subdirectory(BasicGeometry)
add_subdirectory(TextureExplore)
add_subdirectory(AntiAliasing)

## add executeable 
add_executable(twShading ${CMAKE_CURRENT_LIST_DIR}/twShading.cpp)
target_include_directories(twShading PUBLIC 
    ${CMAKE_CURRENT_LIST_DIR}/../ThirdParty
    ${CMAKE_CURRENT_LIST_DIR}/../Common
)
target_link_libraries(twShading twCommon twUiUtil)