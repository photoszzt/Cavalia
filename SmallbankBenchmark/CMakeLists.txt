PROJECT (Cavalia)
file (GLOB SRC_LIST "*.cpp")
set(smallbank_libs libcuckoo mimalloc-static libpapi.a liblz4.a libnuma.a libboost_thread.a pthread rt)


# TARGET_LINK_LIBRARIES (smallbank_benchmark libjemalloc.so libcityhash.so)
ADD_EXECUTABLE (smallbank_benchmark_occ ${SRC_LIST})
TARGET_LINK_LIBRARIES (smallbank_benchmark_occ database_occ_static ${smallbank_libs})
INSTALL (TARGETS smallbank_benchmark_occ
	RUNTIME DESTINATION bin
)

ADD_EXECUTABLE (smallbank_benchmark_silo ${SRC_LIST})
TARGET_LINK_LIBRARIES (smallbank_benchmark_silo database_silo_static ${smallbank_libs})
INSTALL (TARGETS smallbank_benchmark_silo
	RUNTIME DESTINATION bin
)