PROJECT (Cavalia)
file (GLOB SRC_LIST "*.cpp")
ADD_EXECUTABLE (smallbank_benchmark ${SRC_LIST})

# TARGET_LINK_LIBRARIES (smallbank_benchmark libjemalloc.so libcityhash.so)
TARGET_LINK_LIBRARIES (smallbank_benchmark jemalloc_pic database_static liblz4.a libpapi.a libnuma.a libboost_thread.a pthread rt)
INSTALL (TARGETS smallbank_benchmark
	RUNTIME DESTINATION bin
)
