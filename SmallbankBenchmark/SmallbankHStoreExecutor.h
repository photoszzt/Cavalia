#pragma once
#ifndef __CAVALIA_SMALLBANK_BENCHMARK_SMALLBANK_HSTORE_EXECUTOR_H__
#define __CAVALIA_SMALLBANK_BENCHMARK_SMALLBANK_HSTORE_EXECUTOR_H__

#include <Executor/HStoreExecutor.h>

#include "ShardProcedures/AmalgamateShardProcedure.h"
#include "ShardProcedures/BalanceShardProcedure.h"
#include "ShardProcedures/DepositCheckingShardProcedure.h"
#include "ShardProcedures/SendPaymentShardProcedure.h"
#include "ShardProcedures/TransactSavingsShardProcedure.h"
#include "ShardProcedures/WriteCheckShardProcedure.h"

namespace Cavalia {
	namespace Benchmark {
		namespace Smallbank {
			namespace Executor {
				using namespace ShardProcedures;
				template <typename Table> requires IsTable<Table>
				class SmallbankHStoreExecutor : public HStoreExecutor<Table> {
				public:
					using HStoreExecutor<Table>::registers_;
					using HStoreExecutor<Table>::deregisters_;
					SmallbankHStoreExecutor(IORedirector *const redirector, BaseStorageManager<Table> *const storage_manager, BaseLogger *const logger, const HStoreTxnLocation &txn_location) : HStoreExecutor(redirector, storage_manager, logger, txn_location) {}
					virtual ~SmallbankHStoreExecutor() {}

				private:
					virtual void PrepareProcedures() {
						registers_[TupleType::AMALGAMATE] = [](size_t node_id) {
							AmalgamateShardProcedure<Table> *procedure = (AmalgamateShardProcedure<Table>*)MemAllocator::AllocNode(sizeof(AmalgamateShardProcedure<Table>), node_id);
							new(procedure)AmalgamateShardProcedure<Table>(TupleType::AMALGAMATE);
							return procedure;
						};
						deregisters_[TupleType::AMALGAMATE] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(AmalgamateShardProcedure<Table>));
						};
						registers_[TupleType::BALANCE] = [](size_t node_id) {
							BalanceShardProcedure<Table> *procedure = (BalanceShardProcedure<Table>*)MemAllocator::AllocNode(sizeof(BalanceShardProcedure<Table>), node_id);
							new(procedure)BalanceShardProcedure<Table>(TupleType::BALANCE);
							return procedure;
						};
						deregisters_[TupleType::BALANCE] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(BalanceShardProcedure<Table>));
						};
						registers_[TupleType::DEPOSIT_CHECKING] = [](size_t node_id) {
							DepositCheckingShardProcedure<Table> *procedure = (DepositCheckingShardProcedure<Table>*)MemAllocator::AllocNode(sizeof(DepositCheckingShardProcedure<Table>), node_id);
							new(procedure)DepositCheckingShardProcedure<Table>(TupleType::DEPOSIT_CHECKING);
							return procedure;
						};
						deregisters_[TupleType::DEPOSIT_CHECKING] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(DepositCheckingShardProcedure<Table>));
						};
						registers_[TupleType::SEND_PAYMENT] = [](size_t node_id) {
							SendPaymentShardProcedure<Table> *procedure = (SendPaymentShardProcedure<Table>*)MemAllocator::AllocNode(sizeof(SendPaymentShardProcedure<Table>), node_id);
							new(procedure)SendPaymentShardProcedure<Table>(TupleType::SEND_PAYMENT);
							return procedure;
						};
						deregisters_[TupleType::SEND_PAYMENT] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(SendPaymentShardProcedure<Table>));
						};
						registers_[TupleType::TRANSACT_SAVINGS] = [](size_t node_id) {
							TransactSavingsShardProcedure<Table> *procedure = (TransactSavingsShardProcedure<Table>*)MemAllocator::AllocNode(sizeof(TransactSavingsShardProcedure<Table>), node_id);
							new(procedure)TransactSavingsShardProcedure<Table>(TupleType::TRANSACT_SAVINGS);
							return procedure;
						};
						deregisters_[TupleType::TRANSACT_SAVINGS] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(TransactSavingsShardProcedure<Table>));
						};
						registers_[TupleType::WRITE_CHECK] = [](size_t node_id) {
							WriteCheckShardProcedure<Table> *procedure = (WriteCheckShardProcedure<Table>*)MemAllocator::AllocNode(sizeof(WriteCheckShardProcedure<Table>), node_id);
							new(procedure)WriteCheckShardProcedure<Table>(TupleType::WRITE_CHECK);
							return procedure;
						};
						deregisters_[TupleType::WRITE_CHECK] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(WriteCheckShardProcedure<Table>));
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

					virtual void GetPartitionIds(const TxnParam *tuple, std::set<size_t> &part_ids){
						if (tuple->type_ == TupleType::AMALGAMATE){
							part_ids.insert((((AmalgamateParam*)tuple)->custid_0_ - 1) % thread_count_);
							part_ids.insert((((AmalgamateParam*)tuple)->custid_1_ - 1) % thread_count_);
						}
						else if (tuple->type_ == TupleType::DEPOSIT_CHECKING){
							part_ids.insert((((DepositCheckingParam*)tuple)->custid_ - 1) % thread_count_);
						}
						else if (tuple->type_ == TupleType::SEND_PAYMENT){
							part_ids.insert((((SendPaymentParam*)tuple)->custid_0_ - 1) % thread_count_);
							part_ids.insert((((SendPaymentParam*)tuple)->custid_1_ - 1) % thread_count_);
						}
						else if (tuple->type_ == TupleType::TRANSACT_SAVINGS){
							part_ids.insert((((TransactSavingsParam*)tuple)->custid_ - 1) % thread_count_);
						}
						else if (tuple->type_ == TupleType::WRITE_CHECK){
							part_ids.insert((((WriteCheckParam*)tuple)->custid_ - 1) % thread_count_);
						}
						else if (tuple->type_ == TupleType::BALANCE){
							part_ids.insert((((BalanceParam*)tuple)->custid_ - 1) % thread_count_);
						}
					}

				private:
					SmallbankHStoreExecutor(const SmallbankHStoreExecutor &);
					SmallbankHStoreExecutor& operator=(const SmallbankHStoreExecutor &);
				};
			}
		}
	}
}

#endif
