# MacOS 
# brew install glew & glfw & assimp & freeimage
# cmake -G "Unix Makefiles" -DCMAKE_BUILD_TYPE=Debug -DENABLE_DEBUG=ON -DENALBE_EVENT_MULTTHREAD=ON -DBUILD_GTESTS=ON -DCMAKE_TOOLCHAIN_FILE=../mac.toolchain.cmake ..

cmake_minimum_required( VERSION 2.6.3 )
SET(CMAKE_SYSTEM_NAME MacOS)

SET (CMAKE_C_COMPILER             "/usr/bin/clang")
SET (CMAKE_C_FLAGS                "-Wall -std=c11")
SET (CMAKE_C_FLAGS_DEBUG          "-g- O0")
SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

SET (CMAKE_CXX_COMPILER      "/usr/bin/clang++" )
SET (CMAKE_CXX_FLAGS   "-std=c++17")    
SET (CMAKE_CXX_FLAGS   "-g")
SET (CMAKE_CXX_FLAGS   "-Wall")
SET (CMAKE_CXX_FLAGS_DEBUG   "-g -O0" )
SET (CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG " )
