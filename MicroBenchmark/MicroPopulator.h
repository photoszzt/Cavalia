#pragma once
#ifndef __CAVALIA_MICRO_BENCHMARK_MICRO_POPULATOR_H__
#define __CAVALIA_MICRO_BENCHMARK_MICRO_POPULATOR_H__

#include <Benchmark/BenchmarkPopulator.h>

#include "MicroRandomGenerator.h"
#include "MicroScaleParams.h"
#include "MicroInformation.h"

namespace Cavalia{
	namespace Benchmark{
		namespace Micro{
			template <typename Table> requires IsTable<Table>
			class MicroPopulator : public BenchmarkPopulator<Table>{
			public:
				using BenchmarkPopulator<Table>::storage_manager_;
				MicroPopulator(const MicroScaleParams *params, BaseStorageManager<Table> *storage_manager) : BenchmarkPopulator<Table>(storage_manager), num_items_(static_cast<int>(params->scalefactor_*NUM_ITEMS)){}

				virtual ~MicroPopulator(){}

				virtual void StartPopulate(const size_t &min_w_id, const size_t &max_w_id){}

				virtual void StartPopulate(){
					std::cout << "num items=" << num_items_ << std::endl;
					for (int count = 1; count <= num_items_; ++count){
						MicroRecord* record_ptr = GenerateMicroRecord(count);
						InsertMicroRecord(record_ptr);
						delete record_ptr;
						record_ptr = NULL;
					}
				}

			private:
				void InsertMicroRecord(const MicroRecord* record_ptr){
					char *data = new char[MicroSchema::GenerateMicroSchema()->GetSchemaSize()];
					SchemaRecord *record = new SchemaRecord(MicroSchema::GenerateMicroSchema(), data);
					record->SetColumn(0, reinterpret_cast<const char*>(&record_ptr->key_));
					record->SetColumn(1, reinterpret_cast<const char*>(record_ptr->value_));
					storage_manager_->tables_[MICRO_TABLE_ID]->InsertRecord(new TableRecord(record));
				}

				MicroRecord* GenerateMicroRecord(const int& key) const{
					MicroRecord* record = new MicroRecord();
					record->key_ = static_cast<int64_t>(key);
					std::string value = MicroRandomGenerator::GenerateValue(key);
					assert(value.length() == VALUE_LEN);
					memcpy(record->value_, value.c_str(), value.size());
					return record;
				}

			private:
				MicroPopulator(const MicroPopulator&);
				MicroPopulator& operator=(const MicroPopulator&);
			private:
				const int num_items_;
			};
		}
	}
}

#endif
