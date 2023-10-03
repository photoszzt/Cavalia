#include <boost/interprocess/shared_memory_object.hpp>
#include <boost/interprocess/managed_shared_memory.hpp>
#include "oneapi/tbb/concurrent_hash_map.h"
// #include <libcuckoo/cuckoohash_map.hh>

namespace bip = boost::interprocess;

int main(void) {
	typedef bip::allocator<std::string, bip::managed_shared_memory::segment_manager> ShmAlloc;
	// typedef bip::allocator<std::pair<std::string, std::string>, bip::managed_shared_memory::segment_manager> CuckooShmAlloc;
	typedef oneapi::tbb::concurrent_hash_map<std::string, std::string, ShmAlloc> HashMap;
	// typedef libcuckoo::cuckoohash_map<std::string, std::string, std::hash<std::string>, std::equal_to<std::string>, CuckooShmAlloc> CuckooMap;

	// declare memory
	bip::managed_shared_memory segment(bip::create_only, "MySharedMemory", 65536);
	// bip::managed_shared_memory segment2(bip::create_only, "MySharedMemory2", 65536);

	// initialize allocator
	const ShmAlloc alloc_inst(segment.get_segment_manager());
	// const CuckooShmAlloc cuckoo_inst(segment2.get_segment_manager());

	// construct the queue in shared memory
	HashMap *map = segment.construct<HashMap>("data_queue")(alloc_inst);
	// CuckooMap *map_cuckoo = segment2.construct<CuckooMap>("cuckoo_map")(libcuckoo::DEFAULT_SIZE, std::hash<std::string>(), std::equal_to<std::string>(), cuckoo_inst);
}
