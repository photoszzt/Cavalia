#pragma once
#ifndef __CAVALIA_BENCHMARK_FRAMEWORK_BENCHMARK_TABLE_INITIATOR_H__
#define __CAVALIA_BENCHMARK_FRAMEWORK_BENCHMARK_TABLE_INITIATOR_H__

#include "../Storage/BaseStorageManager.h"

namespace Cavalia{
	namespace Benchmark{
		using namespace Cavalia::Database;

		template <typename Table> requires IsTable<Table>
		class BenchmarkTableInitiator{
		public:
			BenchmarkTableInitiator(){}
			virtual ~BenchmarkTableInitiator(){}

			virtual void Initialize(BaseStorageManager<Table> *storage_manager) = 0;

		private:
			BenchmarkTableInitiator(const BenchmarkTableInitiator &);
			BenchmarkTableInitiator& operator=(const BenchmarkTableInitiator &);

		};
	}
}

#endif
