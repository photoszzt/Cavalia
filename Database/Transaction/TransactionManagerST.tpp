#if defined(ST)
#pragma once
#ifndef TRANSACTION_MANAGER_ST_TPP_
#define TRANSACTION_MANAGER_ST_TPP_
#include "TransactionManager.h"

namespace Cavalia{
	namespace Database{
		template <typename Table> requires IsTable<Table>
		bool TransactionManager<Table>::InsertRecord(TxnContext *context, const size_t &table_id, const std::string &primary_key, SchemaRecord *record){
			//storage_manager_->tables_[table_id]->InsertRecord(primary_key, record);
			return true;
		}

		template <typename Table> requires IsTable<Table>
		bool TransactionManager<Table>::SelectRecordCC(TxnContext *context, const size_t &table_id, TableRecord *t_record, SchemaRecord *&record, const AccessType access_type) {
			record = t_record->record_;
			return true;
		}

		template <typename Table> requires IsTable<Table>
		bool TransactionManager<Table>::CommitTransaction(TxnContext *context, TxnParam *param, CharArray &ret_str){
			return true;
		}

		template <typename Table> requires IsTable<Table>
		void TransactionManager<Table>::AbortTransaction() {}
	}
}
#endif // TRANSACTION_MANAGER_ST_TPP_

#endif // ST
