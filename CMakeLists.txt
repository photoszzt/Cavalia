PROJECT (Cavalia)
cmake_minimum_required(VERSION 3.21)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)
add_compile_options("$<$<COMPILE_LANGUAGE:CXX>:-g>")
add_compile_options("$<$<COMPILE_LANGUAGE:C>:-g>")
SET (CMAKE_CXX_FLAGS "-Wall -Wextra -Wno-deprecated-declarations -Wno-unused-parameter -Wno-unused-variable -DPRIMARY_IDX=CuckooIndex -DSND_IDX=StdOrderedIndexMT"
	CACHE INTERNAL "compiler options" FORCE)
SET (CMAKE_CXX_FLAGS_DEBUG "-O0" CACHE INTERNAL "compiler options" FORCE)
SET (CMAKE_CXX_FLAGS_RELEASE "-O3" CACHE INTERNAL "compiler options" FORCE)
IF (NOT CMAKE_BUILD_TYPE)
	SET (CMAKE_BUILD_TYPE "Release")
ENDIF ()
MESSAGE (STATUS "Build type: ${CMAKE_BUILD_TYPE}")

INCLUDE_DIRECTORIES (${PROJECT_SOURCE_DIR}/Common)
INCLUDE_DIRECTORIES (${PROJECT_SOURCE_DIR}/Database)

add_subdirectory(ThirdParty/libcuckoo)
add_subdirectory(ThirdParty/oneTBB)
ADD_SUBDIRECTORY (Database)
ADD_SUBDIRECTORY (TpccBenchmark)
ADD_SUBDIRECTORY (MicroBenchmark)
ADD_SUBDIRECTORY (SmallbankBenchmark)

add_executable(test_tbb_concurrent_hash ${CMAKE_CURRENT_SOURCE_DIR}/tests/test_tbb_concurrent_hash.cpp)
target_link_libraries(test_tbb_concurrent_hash TBB::tbb)
set(tbb_shm_inc_dir
	${CMAKE_CURRENT_SOURCE_DIR}/../../deps/interprocess
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/oneTBB/include/
	${CMAKE_CURRENT_SOURCE_DIR}/ThirdParty/libcuckoo/
)
target_include_directories(test_tbb_concurrent_hash PRIVATE "$<BUILD_INTERFACE:${tbb_shm_inc_dir}>")