#pragma once

#ifndef __PRECOMPILEHEADERLOADED__
#include <time.h>
#include <random>
#endif

namespace etd{
	class Random{
		std::mt19937 rmd;
		Random() : rmd(GetDevRand()())
		{		}
		static std::random_device& GetDevRand() {
			static std::random_device rmd_dev;
			return rmd_dev;
		}
		static Random& Instance() {
			static Random instance;
			return instance;
		}
	public:
		///”ÍˆÍ—”‚Ì¶¬‹@ì¬
		template<typename T>	static auto&& GetIntegerDistribution(T min,T max) {
			return std::move(std::uniform_int_distribution<T>(min, max));
		}
		template<typename T> static auto&& UniRand(T& destribution) {
			return destribution(rmd);
		}
		///”ÍˆÍ—”‚Ì¶¬‹@ì¬
		template<typename T>	static auto  GetRealDistribution(T min, T max) {
			return&& std::uniform_real_distribution<T>(min, max);
		}
		///—”¶¬
		static unsigned Rand(){
			return Instance().rmd();
		}
		static auto& GetMersenne() {
			return Instance().rmd;
		}
	};

}