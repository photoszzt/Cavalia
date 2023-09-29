#pragma once
#ifndef __CAVALIA_SMALLBANK_BENCHMARK_SMALLBANK_SITE_EXECUTOR_H__
#define __CAVALIA_SMALLBANK_BENCHMARK_SMALLBANK_SITE_EXECUTOR_H__

#include <Executor/SiteExecutor.h>

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
				class SmallbankSiteExecutor : public SiteExecutor<Table> {
				public:
					using SiteExecutor<Table>::registers_;
					using SiteExecutor<Table>::deregisters_;
					SmallbankSiteExecutor(IORedirector *const redirector, BaseStorageManager<Table> *const storage_manager, BaseLogger *const logger, const SiteTxnLocation &txn_location) : SiteExecutor(redirector, storage_manager, logger, txn_location) {}
					virtual ~SmallbankSiteExecutor() {}

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

				private:
					SmallbankSiteExecutor(const SmallbankSiteExecutor &);
					SmallbankSiteExecutor& operator=(const SmallbankSiteExecutor &);
				};
			}
		}
	}
}

#endif
