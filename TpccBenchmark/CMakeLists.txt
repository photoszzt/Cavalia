PROJECT (Cavalia)
file (GLOB SRC_LIST "*.cpp")
set(tpcc_libs mimalloc-static libpapi.a liblz4.a libnuma.a libboost_thread.a pthread rt)

# TARGET_LINK_LIBRARIES (tpcc_benchmark libjemalloc.so
# 	libcityhash.so libboost_system.so libboost_date_time.so libboost_thread.so)
ADD_EXECUTABLE (tpcc_benchmark_occ ${SRC_LIST})
TARGET_LINK_LIBRARIES (tpcc_benchmark_occ libboost_thread.a database_occ_static ${tpcc_libs})
INSTALL (TARGETS tpcc_benchmark_occ
	RUNTIME DESTINATION bin
)

ADD_EXECUTABLE (tpcc_benchmark_silo ${SRC_LIST})
TARGET_LINK_LIBRARIES (tpcc_benchmark_silo libboost_thread.a database_silo_static ${tpcc_libs})
INSTALL (TARGETS tpcc_benchmark_silo
	RUNTIME DESTINATION bin
)
