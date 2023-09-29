#pragma once
#ifndef __CAVALIA_BENCHMARK_FRAMEWORK_BENCHMARK_POPULATOR_H__
#define __CAVALIA_BENCHMARK_FRAMEWORK_BENCHMARK_POPULATOR_H__

#include <concepts>
#include <TimeMeasurer.h>
#include "ClassHelper.h"
#include "../Storage/BaseStorageManager.h"

namespace Cavalia{
	namespace Benchmark{
		using namespace Cavalia::Database;
		template<typename Table> requires IsTable<Table>
		class BenchmarkPopulator{
		public:
			BenchmarkPopulator(BaseStorageManager<Table> *storage_manager) : storage_manager_(storage_manager){}
			virtual ~BenchmarkPopulator(){}

			void Start(){
				TimeMeasurer timer;
				timer.StartTimer();
				StartPopulate();
				timer.EndTimer();
				std::cout << "populate elapsed time=" << timer.GetElapsedMilliSeconds() << "ms" << std::endl;
			}

			void Start(const size_t &min_part, const size_t &max_part){
				TimeMeasurer timer;
				timer.StartTimer();
				StartPopulate(min_part, max_part);
				timer.EndTimer();
				std::cout << "populate elapsed time=" << timer.GetElapsedMilliSeconds() << "ms" << std::endl;
			}

			virtual void StartPopulate() = 0;
			virtual void StartPopulate(const size_t &min_part, const size_t &max_part) = 0;

		private:
			BenchmarkPopulator(const BenchmarkPopulator &);
			BenchmarkPopulator& operator=(const BenchmarkPopulator &);

		protected:
			BaseStorageManager<Table> *const storage_manager_;
		};
	}
}

#endif
