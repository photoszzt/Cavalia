#pragma once
#ifndef __CAVALIA_SMALLBANK_BENCHMARK_ATOMIC_PROCEDURES_WRITE_CHECK_PROCEDURE_H__
#define __CAVALIA_SMALLBANK_BENCHMARK_ATOMIC_PROCEDURES_WRITE_CHECK_PROCEDURE_H__

#include <Transaction/StoredProcedure.h>
#include "../SmallbankInformation.h"

namespace Cavalia{
	namespace Benchmark{
		namespace Smallbank{
			namespace AtomicProcedures{
				template <typename Table> requires IsTable<Table>
				class WriteCheckProcedure : public StoredProcedure<Table>{
				public:
					using StoredProcedure<Table>::context_;
					using StoredProcedure<Table>::transaction_manager_;
					WriteCheckProcedure(const size_t &txn_type) : StoredProcedure<Table>(txn_type){
						context_.is_dependent_ = true;
					}
					virtual ~WriteCheckProcedure(){}

					virtual bool Execute(TxnParam *param, CharArray &ret, const ExeContext &exe_context){
						context_.PassContext(exe_context);
						const WriteCheckParam* wc_param = static_cast<const WriteCheckParam*>(param);
						SchemaRecord  *cust_record = NULL;
						DB_QUERY(SelectKeyRecord(&context_, ACCOUNTS_TABLE_ID, std::string((char*)(&wc_param->custid_), sizeof(int64_t)), cust_record, READ_ONLY));
						assert(cust_record != NULL);
						SchemaRecord *savings_record = NULL;
						DB_QUERY(SelectKeyRecord(&context_, SAVINGS_TABLE_ID, std::string((char*)(&wc_param->custid_), sizeof(int64_t)), savings_record, READ_ONLY));
						SchemaRecord *checking_record = NULL;
						DB_QUERY(SelectKeyRecord(&context_, CHECKING_TABLE_ID, std::string((char*)(&wc_param->custid_), sizeof(int64_t)), checking_record, READ_WRITE));
						assert(savings_record != NULL);
						assert(checking_record != NULL);
						float balance = *(float*)(savings_record->GetColumn(1)) + *(float*)(checking_record->GetColumn(1));
						float final_checking = 0.0;
						if (balance < wc_param->amount_) {
							final_checking = balance - (wc_param->amount_ + 1); // maybe negative
							checking_record->UpdateColumn(1, (char*)(&final_checking));
						}
						else{
							final_checking = balance - wc_param->amount_;
							checking_record->UpdateColumn(1, (char*)(&final_checking));
						}
						ret.Memcpy(ret.size_, (char*)(&wc_param->custid_), sizeof(wc_param->custid_));
						ret.size_ += sizeof(wc_param->custid_);
						ret.Memcpy(ret.size_, (char*)(&final_checking), sizeof(final_checking));
						ret.size_ += sizeof(final_checking);
						return transaction_manager_->CommitTransaction(&context_, param, ret);
					}

				private:
					WriteCheckProcedure(const WriteCheckProcedure&);
					WriteCheckProcedure& operator=(const WriteCheckProcedure&);
				};
			}
		}
	}
}

#endif
