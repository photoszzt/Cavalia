#pragma once
#ifndef __CAVALIA_SMALLBANK_BENCHMARK_SMALLBANK_CONCURRENT_EXECUTOR_H__
#define __CAVALIA_SMALLBANK_BENCHMARK_SMALLBANK_CONCURRENT_EXECUTOR_H__

#include <Executor/ConcurrentExecutor.h>

#include "AtomicProcedures/AmalgamateProcedure.h"
#include "AtomicProcedures/BalanceProcedure.h"
#include "AtomicProcedures/DepositCheckingProcedure.h"
#include "AtomicProcedures/SendPaymentProcedure.h"
#include "AtomicProcedures/TransactSavingsProcedure.h"
#include "AtomicProcedures/WriteCheckProcedure.h"

namespace Cavalia{
	namespace Benchmark{
		namespace Smallbank{
			namespace Executor{
				template <typename Table> requires IsTable<Table>
				class SmallbankConcurrentExecutor : public ConcurrentExecutor<Table>{
				public:
					using ConcurrentExecutor<Table>::registers_;
					using ConcurrentExecutor<Table>::deregisters_;
					SmallbankConcurrentExecutor(IORedirector *const redirector, BaseStorageManager<Table> *const storage_manager, BaseLogger *const logger, const size_t &thread_num) : ConcurrentExecutor<Table>(redirector, storage_manager, logger, thread_num){}
					virtual ~SmallbankConcurrentExecutor(){}

				private:
					virtual void PrepareProcedures(){
						using namespace AtomicProcedures;
						registers_[TupleType::AMALGAMATE] = [](size_t node_id){
							AmalgamateProcedure<Table> *procedure = (AmalgamateProcedure<Table>*)MemAllocator::AllocNode(sizeof(AmalgamateProcedure<Table>), node_id);
							new(procedure)AmalgamateProcedure<Table>(TupleType::AMALGAMATE);
							return procedure;
						};
						deregisters_[TupleType::AMALGAMATE] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(AmalgamateProcedure<Table>));
						};
						registers_[TupleType::BALANCE] = [](size_t node_id){
							BalanceProcedure<Table> *procedure = (BalanceProcedure<Table>*)MemAllocator::AllocNode(sizeof(BalanceProcedure<Table>), node_id);
							new(procedure)BalanceProcedure<Table>(TupleType::BALANCE);
							return procedure;
						};
						deregisters_[TupleType::BALANCE] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(BalanceProcedure<Table>));
						};
						registers_[TupleType::DEPOSIT_CHECKING] = [](size_t node_id){
							DepositCheckingProcedure<Table> *procedure = (DepositCheckingProcedure<Table>*)MemAllocator::AllocNode(sizeof(DepositCheckingProcedure<Table>), node_id);
							new(procedure)DepositCheckingProcedure<Table>(TupleType::DEPOSIT_CHECKING);
							return procedure;
						};
						deregisters_[TupleType::DEPOSIT_CHECKING] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(DepositCheckingProcedure<Table>));
						};
						registers_[TupleType::SEND_PAYMENT] = [](size_t node_id){
							SendPaymentProcedure<Table> *procedure = (SendPaymentProcedure<Table>*)MemAllocator::AllocNode(sizeof(SendPaymentProcedure<Table>), node_id);
							new(procedure)SendPaymentProcedure<Table>(TupleType::SEND_PAYMENT);
							return procedure;
						};
						deregisters_[TupleType::SEND_PAYMENT] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(SendPaymentProcedure<Table>));
						};
						registers_[TupleType::TRANSACT_SAVINGS] = [](size_t node_id){
							TransactSavingsProcedure<Table> *procedure = (TransactSavingsProcedure<Table>*)MemAllocator::AllocNode(sizeof(TransactSavingsProcedure<Table>), node_id);
							new(procedure)TransactSavingsProcedure<Table>(TupleType::TRANSACT_SAVINGS);
							return procedure;
						};
						deregisters_[TupleType::TRANSACT_SAVINGS] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(TransactSavingsProcedure<Table>));
						};
						registers_[TupleType::WRITE_CHECK] = [](size_t node_id){
							WriteCheckProcedure<Table> *procedure = (WriteCheckProcedure<Table>*)MemAllocator::AllocNode(sizeof(WriteCheckProcedure<Table>), node_id);
							new(procedure)WriteCheckProcedure<Table>(TupleType::WRITE_CHECK);
							return procedure;
						};
						deregisters_[TupleType::WRITE_CHECK] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(WriteCheckProcedure<Table>));
						};
					}

					virtual TxnParam* DeserializeParam(const size_t &param_type, const CharArray &entry){
						TxnParam *tuple;
						if (param_type == TupleType::AMALGAMATE){
							tuple = new AmalgamateParam();
						}
						else if (param_type == TupleType::DEPOSIT_CHECKING){
							tuple = new DepositCheckingParam();
						}
						else if (param_type == TupleType::SEND_PAYMENT){
							tuple = new SendPaymentParam();
						}
						else if (param_type == TupleType::TRANSACT_SAVINGS){
							tuple = new TransactSavingsParam();
						}
						else if (param_type == TupleType::WRITE_CHECK){
							tuple = new WriteCheckParam();
						}
						else if (param_type == TupleType::BALANCE){
							tuple = new BalanceParam();
						}
						else{
							assert(false);
							return NULL;
						}
						tuple->Deserialize(entry);
						return tuple;
					}

				private:
					SmallbankConcurrentExecutor(const SmallbankConcurrentExecutor &);
					SmallbankConcurrentExecutor& operator=(const SmallbankConcurrentExecutor &);
				};
			}
		}
	}
}

#endif
