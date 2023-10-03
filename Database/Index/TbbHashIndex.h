#pragma once
#ifndef __CAVALIA_DATABASE_TBB_HASH_INDEX_H_
#define __CAVALIA_DATABASE_TBB_HASH_INDEX_H_

#include <oneapi/tbb/concurrent_hash_map.h>

namespace Cavalia{
	namespace Database {
		class TbbHashIndex {
			typedef oneapi::tbb::concurrent_hash_map<std::string, TableRecord*> TbbHashMap;
		public:
			TbbHashIndex() {}
			~TbbHashIndex() {}

			bool InsertRecord(const std::string &key, TableRecord *record) {
				TbbHashMap::accessor a;
				bool ret = hash_index_.insert(a, key);
				a->second = record;
				return ret;
			}

			bool DeleteRecord(const std::string &key) {
				return hash_index_.erase(key);
			}

			TableRecord* SearchRecord(const std::string &key){
				TbbHashMap::accessor a;
				TableRecord *record;
				if (hash_index_.find(a, key) == true){
					return a->second;
				}
				else{
					return NULL;
				}
			}

			size_t GetSize() const {
				return hash_index_.size();
			}

			void SaveCheckpoint(std::ofstream &out_stream, const size_t &record_size){

			}

			TbbHashIndex(const TbbHashIndex &) = delete;
			TbbHashIndex& operator=(const TbbHashIndex &) = delete;

			TbbHashMap hash_index_;
		};
	}
}

#endif // __CAVALIA_DATABASE_TBB_CONCURRENT_UNORDERED_MAP_H_