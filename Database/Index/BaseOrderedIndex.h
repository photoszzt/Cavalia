#pragma once
#ifndef __CAVALIA_DATABASE_BASE_ORDERED_INDEX_H__
#define __CAVALIA_DATABASE_BASE_ORDERED_INDEX_H__

#include <string>
#include "ClassHelper.h"
#include "../Storage/TableRecords.h"

namespace Cavalia{
	namespace Database{
		template <typename IDX>
		concept IsOrderedIndex =
			requires(IDX idx, const std::string& key, const std::string &lower_key,
				const std::string &upper_key,TableRecord *record, TableRecords *records)
			{
				{ idx.InsertRecord(key, record) };
				{ idx.DeleteRecord(key) };
				{ idx.SearchRecord(key) } -> std::same_as<TableRecord*>;
				{ idx.SearchRecords(key, records) };
				{ idx.SearchUpperRecords(key, records) };
				{ idx.SearchLowerRecords(key, records) };
				{ idx.SearchRangeRecords(lower_key, upper_key, records) };
			};

		class BaseOrderedIndex {
		public:
			BaseOrderedIndex(){}
			virtual ~BaseOrderedIndex(){}

			virtual void InsertRecord(const std::string&, TableRecord *) = 0;
			virtual void DeleteRecord(const std::string&) = 0;
			virtual TableRecord* SearchRecord(const std::string&) = 0;
			virtual void SearchRecords(const std::string&, TableRecords*) = 0;
			virtual void SearchUpperRecords(const std::string&, TableRecords*) = 0;
			virtual void SearchLowerRecords(const std::string&, TableRecords*) = 0;
			virtual void SearchRangeRecords(const std::string&, const std::string&, TableRecords*) = 0;

			BaseOrderedIndex(const BaseOrderedIndex &) = delete;
			BaseOrderedIndex& operator=(const BaseOrderedIndex &) = delete;
		};
	}
}

#endif
