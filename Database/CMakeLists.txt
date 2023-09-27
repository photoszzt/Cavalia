PROJECT (Cavalia)
file (GLOB_RECURSE SRC_LIST "*.cc" "*.cpp")
ADD_LIBRARY (database_static STATIC ${SRC_LIST})
ADD_LIBRARY (database_shared SHARED ${SRC_LIST})
TARGET_LINK_LIBRARIES (database_shared liblz4.so)
TARGET_LINK_LIBRARIES (database_shared libpapi.so libjemalloc.so)
target_link_libraries(database_static liblz4.a libpapi.a libjemalloc_pic.a)
SET_TARGET_PROPERTIES (database_static PROPERTIES OUTPUT_NAME "database" CLEAN_DIRECT_OUTPUT 1)
SET_TARGET_PROPERTIES (database_shared PROPERTIES OUTPUT_NAME "database" CLEAN_DIRECT_OUTPUT 1)
INSTALL (TARGETS database_static database_shared
	ARCHIVE DESTINATION bin
	LIBRARY DESTINATION bin
)