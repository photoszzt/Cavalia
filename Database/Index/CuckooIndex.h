#pragma once
#ifndef __CAVALIA_DATABASE_CUCKOO_INDEX_H__
#define __CAVALIA_DATABASE_CUCKOO_INDEX_H__

#include <libcuckoo/cuckoohash_map.hh>
// #include <libcuckoo/city_hasher.hh>
#include "BaseUnorderedIndex.h"

namespace Cavalia{
	namespace Database {
		class CuckooIndex {
			typedef libcuckoo::cuckoohash_map<std::string, TableRecord*> CuckooMap;
		public:
			CuckooIndex(){}
			~CuckooIndex(){}

			bool InsertRecord(const std::string &key, TableRecord *record){
				return hash_index_.insert(key, record);
			}

			bool DeleteRecord(const std::string &key){
				//hash_index_.erase(key);
				return false;
			}

			TableRecord* SearchRecord(const std::string &key){
				TableRecord *record;
				if (hash_index_.find(key, record) == true){
					return record;
				}
				else{
					return NULL;
				}
			}

			size_t GetSize() const {
				return hash_index_.size();
			}

			void SaveCheckpoint(std::ofstream &out_stream, const size_t &record_size){
				auto lt = hash_index_.lock_table();
				for (const auto &it : lt) {
					out_stream.write(it.second->record_->data_ptr_, record_size);
				}
				out_stream.flush();
			}

		// private:
			CuckooIndex(const CuckooIndex &) = delete;
			CuckooIndex& operator=(const CuckooIndex &) = delete;

		// private:
			CuckooMap hash_index_;
		};
		static_assert(IsUnorderedIndex<CuckooIndex>, "");
		CHECK_STD_LAYOUT(CuckooIndex, false);
		CHECK_TRIVIAL(CuckooIndex, false);
	}
}

#endif
