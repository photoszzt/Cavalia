#pragma once
#ifndef __CAVALIA_DATABASE_BASE_UNORDERED_INDEX_H__
#define __CAVALIA_DATABASE_BASE_UNORDERED_INDEX_H__

#include "ClassHelper.h"
#include <string>
#include <fstream>
#include <concepts>
#include "../Storage/TableRecord.h"

namespace Cavalia{
	namespace Database{
		template <typename IDX>
		concept IsUnorderedIndex =
			requires(IDX idx, const std::string& key, TableRecord *record, std::ofstream &out_stream, const size_t &record_size) {
				{ idx.InsertRecord(key, record) } -> std::same_as<bool>;
				{ idx.DeleteRecord(key) } -> std::same_as<bool>;
				{ idx.SearchRecord(key) } -> std::same_as<TableRecord*>;
				{ idx.GetSize() } -> std::same_as<size_t>;
				{ idx.SaveCheckpoint(out_stream, record_size) };
			};

		class BaseUnorderedIndex {
		public:
			BaseUnorderedIndex(){}
			virtual ~BaseUnorderedIndex(){}

			virtual bool InsertRecord(const std::string&, TableRecord *) = 0;
			virtual bool DeleteRecord(const std::string&) = 0;
			virtual TableRecord* SearchRecord(const std::string&) = 0;
			virtual size_t GetSize() const = 0;
			virtual void SaveCheckpoint(std::ofstream &, const size_t &) = 0;

			BaseUnorderedIndex(const BaseUnorderedIndex &) = delete;
			BaseUnorderedIndex& operator=(const BaseUnorderedIndex &) = delete;
		};
	}
}

#endif
