#pragma once
#ifndef __CAVALIA_DATABASE_STD_UNORDERED_INDEX_H__
#define __CAVALIA_DATABASE_STD_UNORDERED_INDEX_H__

#include <unordered_map>
#include "ClassHelper.h"
#include "BaseUnorderedIndex.h"

namespace Cavalia {
	namespace Database {
		class StdUnorderedIndex {
		public:
			StdUnorderedIndex() {}
			~StdUnorderedIndex() {
				// all the records in the table should be deleted by primary index.
				// that is, primary index takes the owership of the data.
				// at current stage, we do not deconstruct entries.
				// tables will only be deleted when the program exits.
				// that is, we rely on the OS to reclaim memory.
			}

			bool InsertRecord(const std::string &key, TableRecord *record) {
				if (hash_index_.find(key) == hash_index_.end()){
					hash_index_[key] = record;
					return true;
				}
				else{
					return false;
				}
			}

			bool DeleteRecord(const std::string &key) {
				if (hash_index_.find(key) == hash_index_.end()){
					return false;
				}
				else{
					//hash_index_.erase(key);
					return true;
				}
			}

			TableRecord* SearchRecord(const std::string &key) {
				if (hash_index_.find(key) == hash_index_.end()) {
					return NULL;
				}
				else {
					return hash_index_.at(key);
				}
			}

			size_t GetSize() const {
				return hash_index_.size();
			}

			void SaveCheckpoint(std::ofstream &out_stream, const size_t &record_size) {
				for (auto &entry : hash_index_){
					out_stream.write(entry.second->record_->data_ptr_, record_size);
				}
				out_stream.flush();
			}

		// private:
			StdUnorderedIndex(const StdUnorderedIndex &) = delete;
			StdUnorderedIndex& operator=(const StdUnorderedIndex &) = delete;

		// protected:
			std::unordered_map<std::string, TableRecord*> hash_index_;
		};
		static_assert(IsUnorderedIndex<StdUnorderedIndex>, "");
		CHECK_STD_LAYOUT(StdUnorderedIndex, true);
		CHECK_TRIVIAL(StdUnorderedIndex, false);
	}
}

#endif
