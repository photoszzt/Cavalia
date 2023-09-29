#pragma once
#ifndef __CAVALIA_DATABASE_STD_UNORDERED_INDEX_MT_H__
#define __CAVALIA_DATABASE_STD_UNORDERED_INDEX_MT_H__

#include <RWLock.h>
#include <ClassHelper.h>
#include "StdUnorderedIndex.h"

namespace Cavalia {
	namespace Database {
		class StdUnorderedIndexMT {
		public:
			StdUnorderedIndexMT() {}
			~StdUnorderedIndexMT() {}

			bool InsertRecord(const std::string &key, TableRecord *record) {
				lock_.AcquireWriteLock();
				if (hash_index_.find(key) == hash_index_.end()){
					hash_index_[key] = record;
					lock_.ReleaseWriteLock();
					return true;
				}
				else{
					lock_.ReleaseWriteLock();
					return false;
				}
			}

			bool DeleteRecord(const std::string &key) {
				lock_.AcquireWriteLock();
				if (hash_index_.find(key) == hash_index_.end()){
					lock_.ReleaseWriteLock();
					return false;
				}
				else{
					hash_index_.erase(key);
					lock_.ReleaseWriteLock();
					return true;
				}
			}

			TableRecord* SearchRecord(const std::string &key) {
				lock_.AcquireReadLock();
				if (hash_index_.find(key) == hash_index_.end()) {
					lock_.ReleaseReadLock();
					return NULL;
				}
				else {
					TableRecord *ret_record = hash_index_.at(key);
					lock_.ReleaseReadLock();
					return ret_record;
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

			DISALLOW_COPY_AND_ASSIGN(StdUnorderedIndexMT);
		// protected:
			RWLock lock_;
			std::unordered_map<std::string, TableRecord*> hash_index_;
		};
		static_assert(IsUnorderedIndex<StdUnorderedIndexMT>, "");
		CHECK_STD_LAYOUT(StdUnorderedIndexMT, true);
		CHECK_TRIVIAL(StdUnorderedIndexMT, false);
	}
}

#endif
