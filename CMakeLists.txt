cmake_minimum_required(VERSION 3.17)
project(Test)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_EXE_LINKER_FLAGS -fuse-ld=lld)
set(CMAKE_SHARED_LINKER_FLAGS -fuse-ld=lld)
set(CMAKE_MODULE_LINKER_FLAGS -fuse-ld=lld)

add_executable(Test main.cpp OctreeNode.hpp Octree.hpp)
set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fprofile-instr-generate -fcoverage-mapping")
#set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fsanitize=fuzzer,leak")

include(CheckIPOSupported)
check_ipo_supported(RESULT result OUTPUT output)
if(result)
    set_property(TARGET Test PROPERTY INTERPROCEDURAL_OPTIMIZATION TRUE)
endif()