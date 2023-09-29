#pragma once
#ifndef __CAVALIA_DATABASE_STD_ORDERED_INDEX_MT_H__
#define __CAVALIA_DATABASE_STD_ORDERED_INDEX_MT_H__

#include <RWLock.h>
#include "StdOrderedIndex.h"

namespace Cavalia {
	namespace Database {
		class StdOrderedIndexMT {
		public:
			StdOrderedIndexMT() {}
			~StdOrderedIndexMT() {}

			void InsertRecord(const std::string &key, TableRecord *record) {
				lock_.AcquireWriteLock();
				index_.insert(std::pair<std::string, TableRecord*>(key, record));
				lock_.ReleaseWriteLock();
			}

			void DeleteRecord(const std::string &key) {
				lock_.AcquireWriteLock();
				index_.erase(key);
				lock_.ReleaseWriteLock();
			}

			TableRecord* SearchRecord(const std::string &key) {
				lock_.AcquireReadLock();
				if (index_.find(key) == index_.end()) {
					lock_.ReleaseReadLock();
					return NULL;
				}
				else {
					TableRecord *ret_record = index_.find(key)->second;
					lock_.ReleaseReadLock();
					return ret_record;
				}
			}

			void SearchRecords(const std::string &key, TableRecords *records) {
				lock_.AcquireReadLock();
				auto range = index_.equal_range(key);
				for (auto it = range.first; it != range.second; ++it) {
					records->InsertRecord(it->second);
				}
				lock_.ReleaseReadLock();
			}

			void SearchUpperRecords(const std::string &key, TableRecords *records){
				lock_.AcquireReadLock();
				for (auto it = index_.lower_bound(key); it != index_.end(); ++it){
					records->InsertRecord(it->second);
				}
				lock_.ReleaseReadLock();
			}

			void SearchLowerRecords(const std::string &key, TableRecords *records){
				lock_.AcquireReadLock();
				for (auto it = index_.begin(); it != index_.upper_bound(key); ++it){
					records->InsertRecord(it->second);
				}
				lock_.ReleaseReadLock();
			}

			void SearchRangeRecords(const std::string &lower_key, const std::string &upper_key, TableRecords *records){
				lock_.AcquireReadLock();
				for (auto it = index_.lower_bound(lower_key); it != index_.upper_bound(upper_key); ++it){
					records->InsertRecord(it->second);
				}
				lock_.ReleaseReadLock();
			}

		// private:
			StdOrderedIndexMT(const StdOrderedIndexMT &) = delete;
			StdOrderedIndexMT& operator=(const StdOrderedIndexMT &) = delete;

		// protected:
			RWLock lock_;
			std::multimap<std::string, TableRecord*> index_;
		};
		static_assert(IsOrderedIndex<StdOrderedIndexMT>);
		static_assert(std::is_trivial_v<StdOrderedIndexMT> == false);
		static_assert(std::is_standard_layout_v<StdOrderedIndexMT> == false);
	}
}

#endif
