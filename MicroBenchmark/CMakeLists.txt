PROJECT (Cavalia)
file (GLOB SRC_LIST "*.cpp")
set(micro_libs mimalloc-static liblz4.a libnuma.a libpapi.a libboost_thread.a pthread rt)
# TARGET_LINK_LIBRARIES (micro_benchmark libjemalloc.so libcityhash.so libpapi.so boost_system boost_date_time libboost_thread)

ADD_EXECUTABLE (micro_benchmark_occ ${SRC_LIST})
TARGET_LINK_LIBRARIES (micro_benchmark_occ database_occ_static ${micro_libs})
INSTALL (TARGETS micro_benchmark_occ
	RUNTIME DESTINATION bin
)

ADD_EXECUTABLE (micro_benchmark_silo ${SRC_LIST})
TARGET_LINK_LIBRARIES (micro_benchmark_silo database_silo_static ${micro_libs})
INSTALL (TARGETS micro_benchmark_silo
	RUNTIME DESTINATION bin
)