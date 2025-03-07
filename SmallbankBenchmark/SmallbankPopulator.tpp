#pragma once
#ifndef __CAVALIA_SMALLBANK_BENCHMARK_SMALLBANK_POPULATOR_TPP__
#define __CAVALIA_SMALLBANK_BENCHMARK_SMALLBANK_POPULATOR_TPP__
#include "SmallbankPopulator.h"


namespace Cavalia{
	namespace Benchmark{
		namespace Smallbank{
			template<typename Table> requires IsTable<Table>
			void SmallbankPopulator<Table>::StartPopulate(){
				std::cout << "num accounts=" << num_accounts_ << std::endl;
				for (int accid = 1; accid <= num_accounts_; ++accid){
					AccountsRecord* account_ptr = GenerateAccountsRecord(accid);
					InsertAccountsRecord(account_ptr);
					delete account_ptr;
					account_ptr = NULL;

					SavingsRecord* saving_ptr = GenerateSavingsRecord(accid);
					InsertSavingsRecord(saving_ptr);
					delete saving_ptr;
					saving_ptr = NULL;

					CheckingRecord* checking_ptr = GenerateCheckingRecord(accid);
					InsertCheckingRecord(checking_ptr);
					delete checking_ptr;
					checking_ptr = NULL;
				}
			}

			template<typename Table> requires IsTable<Table>
			void SmallbankPopulator<Table>::InsertAccountsRecord(const AccountsRecord* record_ptr){
				char *data = new char[SmallbankSchema::GenerateAccountsSchema()->GetSchemaSize()];
				SchemaRecord *accounts_record = new SchemaRecord(SmallbankSchema::GenerateAccountsSchema(), data);
				accounts_record->SetColumn(0, reinterpret_cast<const char*>(&record_ptr->custid_));
				accounts_record->SetColumn(1, reinterpret_cast<const char*>(record_ptr->name_));
				storage_manager_->tables_[ACCOUNTS_TABLE_ID]->InsertRecord(new TableRecord(accounts_record));
			}
			template<typename Table> requires IsTable<Table>
			void SmallbankPopulator<Table>::InsertSavingsRecord(const SavingsRecord* record_ptr){
				char *data = new char[SmallbankSchema::GenerateSavingsSchema()->GetSchemaSize()];
				SchemaRecord *savings_record = new SchemaRecord(SmallbankSchema::GenerateSavingsSchema(), data);
				savings_record->SetColumn(0, reinterpret_cast<const char*>(&record_ptr->custid_));
				savings_record->SetColumn(1, reinterpret_cast<const char*>(&record_ptr->bal_));
				storage_manager_->tables_[SAVINGS_TABLE_ID]->InsertRecord(new TableRecord(savings_record));
			}
			template<typename Table> requires IsTable<Table>
			void SmallbankPopulator<Table>::InsertCheckingRecord(const CheckingRecord* record_ptr){
				char *data = new char[SmallbankSchema::GenerateCheckingSchema()->GetSchemaSize()];
				SchemaRecord *checking_record = new SchemaRecord(SmallbankSchema::GenerateCheckingSchema(), data);
				checking_record->SetColumn(0, reinterpret_cast<const char*>(&record_ptr->custid_));
				checking_record->SetColumn(1, reinterpret_cast<const char*>(&record_ptr->bal_));
				storage_manager_->tables_[CHECKING_TABLE_ID]->InsertRecord(new TableRecord(checking_record));
			}
			template<typename Table> requires IsTable<Table>
			AccountsRecord* SmallbankPopulator<Table>::GenerateAccountsRecord(const int& custid) const{
				AccountsRecord* record = new AccountsRecord();
				record->custid_ = static_cast<int64_t>(custid);
				std::string accountName = SmallbankRandomGenerator::GenerateAccountName(custid);
				assert(accountName.length() == ACCOUNT_NAME_LEN);
				memcpy(record->name_, accountName.c_str(), accountName.size());
				return record;
			}
			template<typename Table> requires IsTable<Table>
			SavingsRecord* SmallbankPopulator<Table>::GenerateSavingsRecord(const int& custid) const{
				SavingsRecord* record = new SavingsRecord();
				record->custid_ = static_cast<int64_t>(custid);
				//int balance = RandomGenerator::GenerateGaussianInteger(MIN_BALANCE, MAX_BALANCE);
				int balance = SmallbankRandomGenerator::GenerateInteger(MIN_BALANCE, MAX_BALANCE);
				record->bal_ = static_cast<float>(balance);
				return record;
			}
			template<typename Table> requires IsTable<Table>
			CheckingRecord* SmallbankPopulator<Table>::GenerateCheckingRecord(const int& custid) const{
				CheckingRecord* record = new CheckingRecord();
				record->custid_ = static_cast<int64_t>(custid);
				//int balance = RandomGenerator::GenerateGaussianInteger(MIN_BALANCE, MAX_BALANCE);
				int balance = SmallbankRandomGenerator::GenerateInteger(MIN_BALANCE, MAX_BALANCE);
				record->bal_ = static_cast<float>(balance);
				return record;
			}
		}
	}
}

#endif // __CAVALIA_SMALLBANK_BENCHMARK_SMALLBANK_POPULATOR_TPP__