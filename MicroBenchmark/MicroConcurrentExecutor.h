#pragma once
#ifndef __CAVALIA_MICRO_BENCHMARK_MICRO_CONCURRENT_EXECUTOR_H__
#define __CAVALIA_MICRO_BENCHMARK_MICRO_CONCURRENT_EXECUTOR_H__

#include <Executor/ConcurrentExecutor.h>

#include "AtomicProcedures/MicroProcedure.h"

namespace Cavalia{
	namespace Benchmark{
		namespace Micro{
			namespace Executor{
				template <typename Table> requires IsTable<Table>
				class MicroConcurrentExecutor : public ConcurrentExecutor<Table>{
				public:
					using ConcurrentExecutor<Table>::registers_;
					using ConcurrentExecutor<Table>::deregisters_;
					MicroConcurrentExecutor(IORedirector *const redirector, BaseStorageManager<Table> *const storage_manager, BaseLogger *const logger, const size_t &thread_num) : ConcurrentExecutor<Table>(redirector, storage_manager, logger, thread_num){}
					virtual ~MicroConcurrentExecutor(){}

				private:
					virtual void PrepareProcedures(){
						using namespace AtomicProcedures;
						registers_[TupleType::MICRO] = [](size_t node_id){
							MicroProcedure<Table> *procedure = (MicroProcedure<Table>*)MemAllocator::AllocNode(sizeof(MicroProcedure<Table>), node_id);
							new(procedure)MicroProcedure<Table>(TupleType::MICRO);
							return procedure;
						};
						deregisters_[TupleType::MICRO] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(MicroProcedure<Table>));
						};
					}

					virtual TxnParam* DeserializeParam(const size_t &param_type, const CharArray &entry){
						TxnParam *tuple;
						if (param_type == TupleType::MICRO){
							tuple = new MicroParam();
						}
						else{
							assert(false);
							return NULL;
						}
						tuple->Deserialize(entry);
						return tuple;
					}

				private:
					MicroConcurrentExecutor(const MicroConcurrentExecutor &);
					MicroConcurrentExecutor& operator=(const MicroConcurrentExecutor &);
				};
			}
		}
	}
}

#endif
