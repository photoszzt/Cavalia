#pragma once
#ifndef __CAVALIA_SMALLBANK_BENCHMARK_SMALLBANK_POPULATOR_H__
#define __CAVALIA_SMALLBANK_BENCHMARK_SMALLBANK_POPULATOR_H__

#include <Benchmark/BenchmarkPopulator.h>

#include "SmallbankRandomGenerator.h"
#include "SmallbankScaleParams.h"
#include "SmallbankInformation.h"

namespace Cavalia{
	namespace Benchmark{
		namespace Smallbank{
			template<typename Table> requires IsTable<Table>
			class SmallbankPopulator : public BenchmarkPopulator<Table>{
			public:
				using BenchmarkPopulator<Table>::storage_manager_;
				SmallbankPopulator(const SmallbankScaleParams *params, BaseStorageManager<Table> *storage_manager) : BenchmarkPopulator<Table>(storage_manager), num_accounts_(static_cast<int>(params->scalefactor_*NUM_ACCOUNTS)){}

				virtual ~SmallbankPopulator(){}

				virtual void StartPopulate();
				virtual void StartPopulate(const size_t &min_w_id, const size_t &max_w_id){
					(void) min_w_id;
					(void) max_w_id;
				}

			private:
				AccountsRecord* GenerateAccountsRecord(const int&) const;
				SavingsRecord* GenerateSavingsRecord(const int&) const;
				CheckingRecord* GenerateCheckingRecord(const int&) const;

				void InsertAccountsRecord(const AccountsRecord*);
				void InsertSavingsRecord(const SavingsRecord*);
				void InsertCheckingRecord(const CheckingRecord*);

			private:
				SmallbankPopulator(const SmallbankPopulator&);
				SmallbankPopulator& operator=(const SmallbankPopulator&);
			private:
				const int num_accounts_;
			};
		}
	}
}

#include "SmallbankPopulator.tpp"

#endif
