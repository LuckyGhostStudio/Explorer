#include "exppch.h"
#include "UUID.h"

#include <random>

namespace Explorer
{
	static std::random_device s_RandomDevice;								//随机设备
	static std::mt19937_64 s_Engine(s_RandomDevice());						//随机数生成引擎
	static std::uniform_int_distribution<uint64_t> s_UniformDistribution;	//随机数分布：均匀分布

	UUID::UUID() :m_UUID(s_UniformDistribution(s_Engine))	//均匀分布生成uint64_t类型随机数
	{

	}
	
	UUID::UUID(uint64_t uuid) :m_UUID(uuid)
	{

	}
}