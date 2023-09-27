PROJECT (Cavalia)
file (GLOB SRC_LIST "*.cpp")
ADD_EXECUTABLE (tpcc_benchmark ${SRC_LIST})

# TARGET_LINK_LIBRARIES (tpcc_benchmark libjemalloc.so
# 	libcityhash.so libboost_system.so libboost_date_time.so libboost_thread.so)
TARGET_LINK_LIBRARIES (tpcc_benchmark jemalloc_pic database_static libpapi.a lz4 libnuma.a libboost_thread.a pthread rt)
INSTALL (TARGETS tpcc_benchmark
	RUNTIME DESTINATION bin
	)
