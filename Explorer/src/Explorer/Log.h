#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

#include <memory>

namespace Explorer
{
	/// <summary>
	/// 日志
	/// </summary>
	class EXPLORER_API Log
	{
	public:
		/// <summary>
		/// 初始化日志
		/// </summary>
		static void Init();

		/// <summary>
		/// 返回内核日志
		/// </summary>
		/// <returns>内核日志</returns>
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

		/// <summary>
		/// 返回客户端日志
		/// </summary>
		/// <returns></returns>
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;		//内核日志
		static std::shared_ptr<spdlog::logger> s_ClientLogger;		//客户端日志
	};
}

//核心日志
#define EXP_CORE_TRACE(...) ::Explorer::Log::GetCoreLogger()->trace(__VA_ARGS__)	//提示日志
#define EXP_CORE_INFO(...) ::Explorer::Log::GetCoreLogger()->info(__VA_ARGS__)		//信息日志
#define EXP_CORE_WARN(...) ::Explorer::Log::GetCoreLogger()->warn(__VA_ARGS__)		//警告日志
#define EXP_CORE_ERROR(...) ::Explorer::Log::GetCoreLogger()->error(__VA_ARGS__)	//错误日志
#define EXP_CORE_FATAL(...) ::Explorer::Log::GetCoreLogger()->fatal(__VA_ARGS__)	//严重错误日志

//客户端日志
#define EXP_TRACE(...) ::Explorer::Log::GetClientLogger()->trace(__VA_ARGS__)	//提示日志
#define EXP_INFO(...) ::Explorer::Log::GetClientLogger()->info(__VA_ARGS__)	//信息日志
#define EXP_WARN(...) ::Explorer::Log::GetClientLogger()->warn(__VA_ARGS__)	//警告日志
#define EXP_ERROR(...) ::Explorer::Log::GetClientLogger()->error(__VA_ARGS__)	//错误日志
#define EXP_FATAL(...) ::Explorer::Log::GetClientLogger()->fatal(__VA_ARGS__)	//严重错误日志