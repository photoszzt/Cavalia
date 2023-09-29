#pragma once
#ifndef __CAVALIA_DATABASE_BASE_TABLE_H__
#define __CAVALIA_DATABASE_BASE_TABLE_H__

#include <AllocatorHelper.h>
#include <vector>
#include <string>
#include <fstream>
#include <concepts>
#include "ClassHelper.h"
#include "TableRecord.h"
#include "TableRecords.h"

namespace Cavalia{
	namespace Database{
		template <typename T>
		concept IsTable =
			requires(T table, const std::string &key, TableRecord *record,
					 TableRecords *records, TableRecord *& record_ret,
					 const size_t &part_id, const size_t &idx_id,
					 const std::string &lower_key, std::string &upper_key) {
				{ table.schema_ptr_ } -> std::same_as<const RecordSchema *const&>;
				{ table.secondary_count_ } -> std::same_as<const size_t&>;
				{ table.GetTableSize() } -> std::same_as<size_t>;
				{ table.InsertRecord(record) } -> std::same_as<bool>;
				{ table.InsertRecord(key, record) } -> std::same_as<bool>;
				{ table.DeleteRecord(record) };
				{ table.DeleteRecord(key, record) };
				{ table.SelectKeyRecord(key, record_ret) };
				{ table.SelectRecord(idx_id, key, record_ret) };
				{ table.SelectRecord(part_id, idx_id, key, record_ret) };
				{ table.SelectRecords(idx_id, key, records) };
				{ table.SelectUpperRecords(idx_id, key, records) };
				{ table.SelectLowerRecords(idx_id, key, records) };
				{ table.SelectRangeRecords(idx_id, lower_key, upper_key, records) };
			};

		template <typename Table> requires IsTable<Table>
		const size_t& GetRecordSize(Table* tab) {
			return tab->schema_ptr_->GetSchemaSize();
		}

		class BaseTable {
		public:
			BaseTable(const RecordSchema * const schema_ptr) : schema_ptr_(schema_ptr), secondary_count_(schema_ptr->GetSecondaryCount()){}
			virtual ~BaseTable(){}

			// get the length of each record.
			const size_t& GetRecordSize() const {
				return schema_ptr_->GetSchemaSize();
			}

			// get the number of records in this table.
			virtual size_t GetTableSize() const = 0;

			virtual bool InsertRecord(TableRecord *record) = 0;
			virtual bool InsertRecord(const std::string &key, TableRecord *record) = 0;
			virtual void DeleteRecord(TableRecord *record) = 0;
			virtual void DeleteRecord(const std::string &key, TableRecord *record) = 0;

			///////////////////NEW API//////////////////
			virtual void SelectKeyRecord(const std::string &key, TableRecord *&record) const = 0;
			virtual void SelectKeyRecord(const size_t &part_id, const std::string &key, TableRecord *&record) const = 0;
			virtual void SelectRecord(const size_t &idx_id, const std::string &key, TableRecord *&record) const = 0;
			virtual void SelectRecord(const size_t &part_id, const size_t &idx_id, const std::string &key, TableRecord *&record) const = 0;
			virtual void SelectRecords(const size_t &idx_id, const std::string &key, TableRecords *records) const = 0;
			virtual void SelectRecords(const size_t &part_id, const size_t &idx_id, const std::string &key, TableRecords *records) const = 0;
			virtual void SelectUpperRecords(const size_t &idx_id, const std::string &key, TableRecords *records) const = 0;
			virtual void SelectUpperRecords(const size_t part_id, const size_t &idx_id, const std::string &key, TableRecords *records) const = 0;
			virtual void SelectLowerRecords(const size_t &idx_id, const std::string &key, TableRecords *records) const = 0;
			virtual void SelectLowerRecords(const size_t part_id, const size_t &idx_id, const std::string &key, TableRecords *records) const = 0;
			virtual void SelectRangeRecords(const size_t &idx_id, const std::string &lower_key, std::string &upper_key, TableRecords *records) const = 0;
			virtual void SelectRangeRecords(const size_t part_id, const size_t &idx_id, const std::string &lower_key, const std::string &upper_key, TableRecords *records) const = 0;

			virtual void SaveCheckpoint(std::ofstream &out_stream) = 0;
			virtual void ReloadCheckpoint(std::ifstream &in_stream) = 0;

			BaseTable(const BaseTable&) = delete;
			BaseTable & operator=(const BaseTable&) = delete;

		protected:
			const RecordSchema *const schema_ptr_;
			const size_t secondary_count_;
		};
	}
}

#endif