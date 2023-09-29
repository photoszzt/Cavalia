#pragma once
#ifndef __CAVALIA_TPCC_BENCHMARK_TPCC_CONCURRENT_EXECUTOR_H__
#define __CAVALIA_TPCC_BENCHMARK_TPCC_CONCURRENT_EXECUTOR_H__

#include <Executor/ConcurrentExecutor.h>

#include "AtomicProcedures/DeliveryProcedure.h"
#include "AtomicProcedures/NewOrderProcedure.h"
#include "AtomicProcedures/PaymentProcedure.h"
#include "AtomicProcedures/OrderStatusProcedure.h"
#include "AtomicProcedures/StockLevelProcedure.h"

namespace Cavalia{
	namespace Benchmark{
		namespace Tpcc{
			namespace Executor{
				template <typename Table> requires IsTable<Table>
				class TpccConcurrentExecutor : public ConcurrentExecutor<Table>{
				public:
					using ConcurrentExecutor<Table>::registers_;
					using ConcurrentExecutor<Table>::deregisters_;
					TpccConcurrentExecutor(IORedirector *const redirector, BaseStorageManager<Table> *const storage_manager, BaseLogger *const logger, const size_t &thread_num) : ConcurrentExecutor<Table>(redirector, storage_manager, logger, thread_num){}
					virtual ~TpccConcurrentExecutor(){}

				private:
					virtual void PrepareProcedures(){
						using namespace AtomicProcedures;
						registers_[TupleType::DELIVERY] = [](size_t node_id) {
							DeliveryProcedure<Table> *procedure = (DeliveryProcedure<Table>*)MemAllocator::AllocNode(sizeof(DeliveryProcedure<Table>), node_id);
							new(procedure)DeliveryProcedure<Table>(TupleType::DELIVERY);
							return procedure;
						};
						deregisters_[TupleType::DELIVERY] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(DeliveryProcedure<Table>));
						};
						registers_[TupleType::NEW_ORDER] = [](size_t node_id) {
							NewOrderProcedure<Table> *procedure = (NewOrderProcedure<Table>*)MemAllocator::AllocNode(sizeof(NewOrderProcedure<Table>), node_id);
							new(procedure)NewOrderProcedure<Table>(TupleType::NEW_ORDER);
							return procedure;
						};
						deregisters_[TupleType::NEW_ORDER] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(NewOrderProcedure<Table>));
						};
						registers_[TupleType::PAYMENT] = [](size_t node_id) {
							PaymentProcedure<Table> *procedure = (PaymentProcedure<Table>*)MemAllocator::AllocNode(sizeof(PaymentProcedure<Table>), node_id);
							new(procedure)PaymentProcedure<Table>(TupleType::PAYMENT);
							return procedure;
						};
						deregisters_[TupleType::PAYMENT] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(PaymentProcedure<Table>));
						};
						registers_[TupleType::ORDER_STATUS] = [](size_t node_id) {
							OrderStatusProcedure<Table> *procedure = (OrderStatusProcedure<Table>*)MemAllocator::AllocNode(sizeof(OrderStatusProcedure<Table>), node_id);
							new(procedure)OrderStatusProcedure<Table>(TupleType::ORDER_STATUS);
							return procedure;
						};
						deregisters_[TupleType::ORDER_STATUS] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(OrderStatusProcedure<Table>));
						};
						registers_[TupleType::STOCK_LEVEL] = [](size_t node_id) {
							StockLevelProcedure<Table> *procedure = (StockLevelProcedure<Table>*)MemAllocator::AllocNode(sizeof(StockLevelProcedure<Table>), node_id);
							new(procedure)StockLevelProcedure<Table>(TupleType::STOCK_LEVEL);
							return procedure;
						};
						deregisters_[TupleType::STOCK_LEVEL] = [](char *ptr){
							MemAllocator::FreeNode(ptr, sizeof(StockLevelProcedure<Table>));
						};
					}

					virtual TxnParam* DeserializeParam(const size_t &param_type, const CharArray &entry){
						TxnParam *tuple;
						if (param_type == TupleType::DELIVERY){
							tuple = new DeliveryParam();
						}
						else if (param_type == TupleType::NEW_ORDER){
							tuple = new NewOrderParam();
						}
						else if (param_type == TupleType::PAYMENT){
							tuple = new PaymentParam();
						}
						else if (param_type == TupleType::ORDER_STATUS){
							tuple = new OrderStatusParam();
						}
						else if (param_type == TupleType::STOCK_LEVEL){
							tuple = new StockLevelParam();
						}
						else{
							assert(false);
							return NULL;
						}
						tuple->Deserialize(entry);
						return tuple;
					}

				private:
					TpccConcurrentExecutor(const TpccConcurrentExecutor &);
					TpccConcurrentExecutor& operator=(const TpccConcurrentExecutor &);
				};
			}
		}
	}
}

#endif
