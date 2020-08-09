

SET(MaterialTests_SRCs
    ${CMAKE_CURRENT_LIST_DIR}/twMaterialTest.cpp
)

SET(EventlTests_SRCs
    ${CMAKE_CURRENT_LIST_DIR}/twEventTest.cpp
)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall")

add_executable(MaterialTests ${MaterialTests_SRCs})
    target_include_directories(MaterialTests PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/../../Include/Common
)

add_executable(EventTests ${EventlTests_SRCs})
    target_include_directories(EventTests PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/../../Include/Common
        ${CMAKE_CURRENT_LIST_DIR}/../../Include/Event
)


if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_include_directories(MaterialTests PUBLIC 
        # ${GTESTINCLUDEDIR}
        E:\\MicroqqStudio\\googletest\\googletest\\include
    )
    target_include_directories(EventTests PUBLIC 
        # ${GTESTINCLUDEDIR}
        E:\\MicroqqStudio\\googletest\\googletest\\include
    )
    target_link_libraries(MaterialTests twCommon)
    target_link_libraries(MaterialTests E:\\MicroqqStudio\\googletest\\build\\lib\\libgtest.a)
    target_link_libraries(MaterialTests E:\\MicroqqStudio\\googletest\\build\\lib\\libgtest_main.a)
    # target_link_libraries(MaterialTests ${GTESTLIBDIR}/libgtest.a)
    # target_link_libraries(MaterialTests ${GTESTLIBDIR}/libgtest_main.a)

    target_link_libraries(EventTests twCommon)
    target_link_libraries(EventTests twEvent)
    target_link_libraries(EventTests E:\\MicroqqStudio\\googletest\\build\\lib\\libgtest.a)
    target_link_libraries(EventTests E:\\MicroqqStudio\\googletest\\build\\lib\\libgtest_main.a)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(MaterialTests twCommon)
    target_link_libraries(MaterialTests ${GTEST_BOTH_LIBRARIES})
    target_link_libraries(MaterialTests ${CMAKE_THREAD_LIBS_INIT})

    target_link_libraries(EventTests twCommon)
    target_link_libraries(EventTests ${GTEST_BOTH_LIBRARIES})
    target_link_libraries(EventTests ${CMAKE_THREAD_LIBS_INIT})
endif()
