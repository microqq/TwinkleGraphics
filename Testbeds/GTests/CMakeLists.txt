

    SET(GTests_SRCs
    ${CMAKE_CURRENT_LIST_DIR}/twMaterialTest.cpp
    )

    # set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall")

    add_executable(MaterialTests ${GTests_SRCs})
    # target_include_directories(MaterialTests PUBLIC
    #        ${CMAKE_CURRENT_LIST_DIR}/../../Common
    # )

    target_link_libraries(MaterialTests twCommon)
    target_link_libraries(MaterialTests ${GTEST_BOTH_LIBRARIES})
    target_link_libraries(MaterialTests ${CMAKE_THREAD_LIBS_INIT})
