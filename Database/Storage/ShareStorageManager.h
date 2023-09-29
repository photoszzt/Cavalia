#pragma once
#ifndef __CAVALIA_DATABASE_SHARE_STORAGE_MANAGER_H__
#define __CAVALIA_DATABASE_SHARE_STORAGE_MANAGER_H__

#include "BaseStorageManager.h"
#include <type_traits>
#include "ShareTable.h"

namespace Cavalia{
	namespace Database{
		class ShareStorageManager : public BaseStorageManager<ShareTable> {
		public:
			ShareStorageManager(const std::string &filename, bool is_thread_safe) : BaseStorageManager(filename), is_thread_safe_(is_thread_safe){}
			virtual ~ShareStorageManager(){}

			virtual void RegisterTables(const std::unordered_map<size_t, RecordSchema*> &schemas){
				table_count_ = schemas.size();
				tables_ = new ShareTable*[table_count_];
				for (auto &entry : schemas){
					tables_[entry.first] = new ShareTable(entry.second, is_thread_safe_);
				}
			}

		private:
			bool is_thread_safe_;
		};
		static_assert(IsStorageManager<ShareStorageManager, ShareTable>);
	}
}

#endif
