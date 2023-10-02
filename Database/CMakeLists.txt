PROJECT (Cavalia)
file (GLOB_RECURSE SRC_LIST "*.cc" "*.cpp")
set(shared_deps liblz4.so libpapi.so libcuckoo)
set(static_deps liblz4.a libpapi.a libcuckoo)
set(shared_deps ${shared_deps} mimalloc)
set(static_deps ${static_deps} mimalloc-static)

add_library (database_occ_static STATIC ${SRC_LIST})
add_library (database_occ_shared SHARED ${SRC_LIST})
target_link_libraries (database_occ_shared ${shared_deps})
target_link_libraries(database_occ_static ${static_deps})
target_compile_definitions(database_occ_shared PUBLIC OCC)
target_compile_definitions(database_occ_static PUBLIC OCC)
set_target_properties (database_occ_static PROPERTIES OUTPUT_NAME "database_occ" CLEAN_DIRECT_OUTPUT 1)
set_target_properties (database_occ_shared PROPERTIES OUTPUT_NAME "database_occ" CLEAN_DIRECT_OUTPUT 1)
INSTALL (TARGETS database_occ_static database_occ_shared
	ARCHIVE DESTINATION bin
	LIBRARY DESTINATION bin
)

add_library (database_silo_static STATIC ${SRC_LIST})
add_library (database_silo_shared SHARED ${SRC_LIST})
target_link_libraries (database_silo_shared ${shared_deps})
target_link_libraries(database_silo_static ${static_deps})
target_compile_definitions(database_silo_shared PUBLIC SILO)
target_compile_definitions(database_silo_static PUBLIC SILO)
set_target_properties (database_silo_static PROPERTIES OUTPUT_NAME "database_silo" CLEAN_DIRECT_OUTPUT 1)
set_target_properties (database_silo_shared PROPERTIES OUTPUT_NAME "database_silo" CLEAN_DIRECT_OUTPUT 1)
INSTALL (TARGETS database_silo_static database_silo_shared
	ARCHIVE DESTINATION bin
	LIBRARY DESTINATION bin
)