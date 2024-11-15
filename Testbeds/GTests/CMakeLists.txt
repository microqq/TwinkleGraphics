
# git clone https://github.com/google/googletest.git
# mkdir build & cd build
# cmake -G "MinGW Makefiles" -DCMAKE_CXX_FLAGS=-std=c++11 ..
# make
# SET System Variable %GTESTINCLUDE% & %GTESLIB%
# VSCode: Install GoogleTest Adapter extension

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(Tests_SRCs
    ${CMAKE_CURRENT_LIST_DIR}/twEventTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twMaterialTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twRingBufferTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twThreadTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twReaderTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/twBoundingVolumeTest.cpp
    ${CMAKE_CURRENT_LIST_DIR}/main.cpp
)

add_executable(TWTests ${Tests_SRCs})
    target_include_directories(TWTests PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/../../Include/Common
        ${CMAKE_CURRENT_LIST_DIR}/../../Include/Event
        ${CMAKE_CURRENT_LIST_DIR}
        ${GTESTINCLUDEDIR}
)

if(WIN32)
    target_link_libraries(TWTests twGraphics)
    target_link_libraries(TWTests ${GTESTLIBDIR}/libgtest.a)
    target_link_libraries(TWTests ${GTESTLIBDIR}/libgtest_main.a)
elseif(LINUX)
    target_link_libraries(TWTests twGraphics)
    target_link_libraries(TWTests ${GTEST_BOTH_LIBRARIES})
    target_link_libraries(TWTests ${CMAKE_THREAD_LIBS_INIT})
else()
endif(WIN32)



