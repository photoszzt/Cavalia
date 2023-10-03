#pragma once
#ifndef __CAVALIA_DATABASE_SHARE_TABLE_H__
#define __CAVALIA_DATABASE_SHARE_TABLE_H__

#include <concepts>
#include "BaseTable.h"
#include "ClassHelper.h"
#include "../Index/StdUnorderedIndex.h"
#include "../Index/StdUnorderedIndexMT.h"
#include "../Index/StdOrderedIndex.h"
#include "../Index/StdOrderedIndexMT.h"
#include "../Index/CuckooIndex.h"
#include "../Index/TbbHashIndex.h"

namespace Cavalia{
	namespace Database{
		class ShareTable {
		public:
			ShareTable(const RecordSchema * const schema_ptr, bool is_thread_safe) : schema_ptr_(schema_ptr), secondary_count_(schema_ptr->GetSecondaryCount()) {
				// if (is_thread_safe == true){
					primary_index_ = new PRIMARY_IDX();
					secondary_indexes_ = new SND_IDX*[secondary_count_];
					for (size_t i = 0; i < secondary_count_; ++i){
						secondary_indexes_[i] = new SND_IDX();
					}
				// }
				// else{
				// 	primary_index_ = new StdUnorderedIndex();
				// 	secondary_indexes_ = new BaseOrderedIndex*[secondary_count_];
				// 	for (size_t i = 0; i < secondary_count_; ++i){
				// 		secondary_indexes_[i] = new StdOrderedIndex();
				// 	}
				// }
			}

			~ShareTable(){
				// records in the table is released by primary index.
				delete primary_index_;
				primary_index_ = NULL;
				for (size_t i = 0; i < secondary_count_; ++i){
					delete secondary_indexes_[i];
					secondary_indexes_[i] = NULL;
				}
				delete[] secondary_indexes_;
				secondary_indexes_ = NULL;
			}

			// get the number of records in this table.
			size_t GetTableSize() const {
				return primary_index_->GetSize();
			}

			///////////////////INSERT//////////////////
			bool InsertRecord(TableRecord *record){
				SchemaRecord *record_ptr = record->record_;
				if (primary_index_->InsertRecord(record_ptr->GetPrimaryKey(), record) == true){
					// build secondary index here
					for (size_t i = 0; i < secondary_count_; ++i){
						secondary_indexes_[i]->InsertRecord(record_ptr->GetSecondaryKey(i), record);
					}
					return true;
				}
				else{
					return false;
				}
			}

			bool InsertRecord(const std::string &primary_key, TableRecord *record){
				if (primary_index_->InsertRecord(primary_key, record) == true){
					SchemaRecord *record_ptr = record->record_;
					// build secondary index here
					for (size_t i = 0; i < secondary_count_; ++i){
						secondary_indexes_[i]->InsertRecord(record_ptr->GetSecondaryKey(i), record);
					}
					return true;
				}
				else{
					return false;
				}
			}

			/////////////////////DELETE//////////////////
			void DeleteRecord(TableRecord *record){
				SchemaRecord *record_ptr = record->record_;
				primary_index_->DeleteRecord(record_ptr->GetPrimaryKey());
				// update secondary index here
				for (size_t i = 0; i < secondary_count_; ++i){
					secondary_indexes_[i]->DeleteRecord(record_ptr->GetSecondaryKey(i));
				}
				// ========================IMPORTANT========================
				// TODO: deletion should rely on a garbage collector. potential memory leak!
			}

			void DeleteRecord(const std::string &primary_key, TableRecord *record) {
				primary_index_->DeleteRecord(primary_key);
				SchemaRecord *record_ptr = record->record_;
				for (size_t i = 0; i < secondary_count_; ++i) {
					secondary_indexes_[i]->DeleteRecord(record_ptr->GetSecondaryKey(i));
				}
			}

			///////////////////NEW API//////////////////
			void SelectKeyRecord(const std::string &primary_key, TableRecord *&record) const {
				record = primary_index_->SearchRecord(primary_key);
			}

			void SelectKeyRecord(const size_t &part_id, const std::string &primary_key, TableRecord *&record) const {
				(void) part_id;
				(void) primary_key;
				(void) record;
				assert(false);
			}

			void SelectRecord(const size_t &idx_id, const std::string &key, TableRecord *&record) const {
				record = secondary_indexes_[idx_id]->SearchRecord(key);
			}

			void SelectRecord(const size_t &part_id, const size_t &idx_id, const std::string &key, TableRecord *&record) const {
				(void) part_id;
				(void) idx_id;
				(void) key;
				(void) record;
				assert(false);
			}

			void SelectRecords(const size_t &idx_id, const std::string &key, TableRecords *records) const {
				secondary_indexes_[idx_id]->SearchRecords(key, records);
			}

			void SelectRecords(const size_t &part_id, const size_t &idx_id, const std::string &key, TableRecords *records) const {
				(void) part_id;
				(void) idx_id;
				(void) key;
				(void) records;
				assert(false);
			}

			void SelectUpperRecords(const size_t &idx_id, const std::string &key, TableRecords *records) const {
				secondary_indexes_[idx_id]->SearchUpperRecords(key, records);
			}

			void SelectUpperRecords(const size_t part_id, const size_t &idx_id, const std::string &key, TableRecords *records) const {
				(void) part_id;
				(void) idx_id;
				(void) key;
				(void) records;
				assert(false);
			}

			void SelectLowerRecords(const size_t &idx_id, const std::string &key, TableRecords *records) const {
				secondary_indexes_[idx_id]->SearchLowerRecords(key, records);
			}

			void SelectLowerRecords(const size_t part_id, const size_t &idx_id, const std::string &key, TableRecords *records) const {
				(void) part_id;
				(void) idx_id;
				(void) key;
				(void) records;
				assert(false);
			}

			void SelectRangeRecords(const size_t &idx_id, const std::string &lower_key, std::string &upper_key, TableRecords *records) const {
				secondary_indexes_[idx_id]->SearchRangeRecords(lower_key, upper_key, records);
			}

			void SelectRangeRecords(const size_t part_id, const size_t &idx_id, const std::string &lower_key, const std::string &upper_key, TableRecords *records) const {
				(void) part_id;
				(void) idx_id;
				(void) lower_key;
				(void) upper_key;
				(void) records;
				assert(false);
			}


			void SaveCheckpoint(std::ofstream &out_stream){
				size_t record_size = schema_ptr_->GetSchemaSize();
				primary_index_->SaveCheckpoint(out_stream, record_size);
			}

			void ReloadCheckpoint(std::ifstream &in_stream){
				size_t record_size = schema_ptr_->GetSchemaSize();
				in_stream.seekg(0, std::ios::end);
				size_t file_size = static_cast<size_t>(in_stream.tellg());
				in_stream.seekg(0, std::ios::beg);
				size_t file_pos = 0;
				while (file_pos < file_size){
					char *entry = new char[record_size];
					in_stream.read(entry, record_size);
					TableRecord *record_ptr = new TableRecord(new SchemaRecord(schema_ptr_, entry));
					//if (schema_ptr_->GetTableId() >= 2 && schema_ptr_->GetTableId() <= 5)
					//	record_ptr->content_.SetHot(true);
					InsertRecord(record_ptr);
					file_pos += record_size;
				}
			}
			DISALLOW_COPY_AND_ASSIGN(ShareTable);

			PRIMARY_IDX *primary_index_;
			SND_IDX **secondary_indexes_;
			const RecordSchema *const schema_ptr_;
			const size_t secondary_count_;
		};
		static_assert(IsTable<ShareTable>);
		static_assert(std::is_trivial_v<ShareTable> == false);
		static_assert(std::is_standard_layout_v<ShareTable> == true);
	}
}

#endif
