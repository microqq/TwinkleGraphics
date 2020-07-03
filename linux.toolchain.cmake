# Ubuntu 16.04
# install glew & glfw
# install clang(all key package)
# mkdir build & cd build
# cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../linux.toolchain.cmake ..

cmake_minimum_required( VERSION 2.6.3 )
SET(CMAKE_SYSTEM_NAME Linux )

SET (CMAKE_C_COMPILER             "/usr/bin/clang-8")
SET (CMAKE_C_FLAGS                "-Wall -std=c11")
SET (CMAKE_C_FLAGS_DEBUG          "-g- O0")
SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")
 
SET (CMAKE_CXX_COMPILER      "/usr/bin/clang++-8" )
SET (CMAKE_CXX_FLAGS   "-std=c++17")    
SET (CMAKE_CXX_FLAGS   "-g")
SET (CMAKE_CXX_FLAGS   "-Wall")
SET (CMAKE_CXX_FLAGS_DEBUG   "-g -O0" )
SET (CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG " )

SET (CMAKE_AR "/usr/bin/llvm-ar-8")
SET (CMAKE_NM "/usr/bin/llvm-nm-8")
SET (CMAKE_LINKER "/usr/bin/llvm-link-8")
SET (CMAKE_OBJDUMP "/usr/bin/llvm-objdump-8")
SET (CMAKE_RANLIB "/usr/bin/llvm-ranlib-8") 