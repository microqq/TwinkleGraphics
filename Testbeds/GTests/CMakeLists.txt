
# git clone https://github.com/google/googletest.git
# mkdir build & cd build
# cmake -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS=-std=c++11 ..
# make
# SET System Variable %GTESTINCLUDE% & %GTESLIB%
# VSCode: Install GoogleTest Adapter extension

set(Tests_SRCs
    ${CMAKE_CURRENT_LIST_DIR}/twEventTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twMaterialTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twRingBufferTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twThreadTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twReaderTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++11 -Wall")

add_executable(TWTests ${Tests_SRCs})
    target_include_directories(TWTests PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/../../Include/Common
        ${CMAKE_CURRENT_LIST_DIR}/../../Include/Event
        ${CMAKE_CURRENT_LIST_DIR}
        ${GTESTINCLUDEDIR}
)


if(${CMAKE_SYSTEM_NAME} STREQUAL "Windows")
    target_link_libraries(TWTests twEvent)
    target_link_libraries(TWTests ${GTESTLIBDIR}/libgtest.a)
    target_link_libraries(TWTests ${GTESTLIBDIR}/libgtest_main.a)
endif()

if(${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    target_link_libraries(TWTests twCommon)
    target_link_libraries(TWTests ${GTEST_BOTH_LIBRARIES})
    target_link_libraries(TWTests ${CMAKE_THREAD_LIBS_INIT})
endif()



