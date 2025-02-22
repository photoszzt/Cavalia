#pragma once
#ifndef __CAVALIA_DATABASE_BTREE_INDEX_H__
#define __CAVALIA_DATABASE_BTREE_INDEX_H__

#include "ClassHelper.h"
#include <BTree/btree_map.h>
// #include "BaseOrderedIndex.h"
#include "../Storage/TableRecords.h"

namespace Cavalia {
	namespace Database {
		class BtreeIndex {
		public:
			BtreeIndex(){}
			~BtreeIndex() {}

			void InsertRecord(const std::string &key, TableRecord *record) {
				index_.insert(std::pair<std::string, TableRecord*>(key, record));
			}

			void DeleteRecord(const std::string &key) {
				index_.erase(key);
			}

			TableRecord* SearchRecord(const std::string &key) {
				if (index_.find(key) == index_.end()) {
					return NULL;
				}
				else {
					return index_.find(key)->second;
				}
			}

			void SearchRecords(const std::string &key, TableRecords *records) {
				size_t num = index_.count(key);
				if (num == 0) {
					return;
				}
				else {
					auto range = index_.equal_range(key);
					for (auto it = range.first; it != range.second; ++it) {
						records->InsertRecord(it->second);
					}
					return;
				}
			}


			BtreeIndex(const BtreeIndex &) = delete;
			BtreeIndex& operator=(const BtreeIndex &) = delete;

		// protected:
			btree::btree_multimap<std::string, TableRecord*> index_;
		};
		static_assert(std::is_trivial_v<BtreeIndex> == false);
		static_assert(std::is_standard_layout_v<BtreeIndex> == true);
	}
}

#endif
