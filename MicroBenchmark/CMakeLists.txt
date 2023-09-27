PROJECT (Cavalia)
file (GLOB SRC_LIST "*.cpp")
ADD_EXECUTABLE (micro_benchmark ${SRC_LIST})

# TARGET_LINK_LIBRARIES (micro_benchmark libjemalloc.so libcityhash.so libpapi.so boost_system boost_date_time libboost_thread)
TARGET_LINK_LIBRARIES (micro_benchmark jemalloc_pic database_static liblz4.a libnuma.a libpapi.a libboost_thread.a pthread rt)
INSTALL (TARGETS micro_benchmark
	RUNTIME DESTINATION bin
)
