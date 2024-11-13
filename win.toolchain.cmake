# Windows 10
# install mingw-w64(set env, copy mingw-make.exe to make.exe)
# cmake build: 
# cd $project_dir
# mkdir build & cd build
<<<<<<< Updated upstream
# cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DCMAKE_TOOLCHAIN_FILE=../win.toolchain.cmake ..

cmake_minimum_required( VERSION 3.30.0 )
=======
# cmake -G "MinGW Makefiles" -DCMAKE_BUILD_TYPE=Debug -DUSE_DEBUG=ON -DBUILD_GTESTS=ON -DCMAKE_TOOLCHAIN_FILE=../win.toolchain.cmake ..
cmake_minimum_required( VERSION 2.6.3 )
>>>>>>> Stashed changes
SET(CMAKE_SYSTEM_NAME Windows)

SET (CMAKE_C_FLAGS                "-Wall -std=c11")
SET (CMAKE_C_FLAGS_DEBUG          "-g -O0")
SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")
 
SET (CMAKE_CXX_FLAGS   "-std=c++17")
SET (CMAKE_CXX_FLAGS   "-g")
SET (CMAKE_CXX_FLAGS   "${CMAKE_CXX_FLAGS} -Wall -Wextra")
SET (CMAKE_CXX_FLAGS_DEBUG   "-g -O0" )
SET (CMAKE_CXX_FLAGS_RELEASE "-O2 -DNDEBUG " )