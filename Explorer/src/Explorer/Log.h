#pragma once

#include "Core.h"
#include "spdlog/spdlog.h"

#include <memory>

namespace Explorer
{
	/// <summary>
	/// ��־
	/// </summary>
	class EXPLORER_API Log
	{
	public:
		/// <summary>
		/// ��ʼ����־
		/// </summary>
		static void Init();

		/// <summary>
		/// �����ں���־
		/// </summary>
		/// <returns>�ں���־</returns>
		inline static std::shared_ptr<spdlog::logger>& GetCoreLogger() { return s_CoreLogger; }

		/// <summary>
		/// ���ؿͻ�����־
		/// </summary>
		/// <returns></returns>
		inline static std::shared_ptr<spdlog::logger>& GetClientLogger() { return s_ClientLogger; }
	private:
		static std::shared_ptr<spdlog::logger> s_CoreLogger;		//�ں���־
		static std::shared_ptr<spdlog::logger> s_ClientLogger;		//�ͻ�����־
	};
}

//������־
#define EXP_CORE_TRACE(...) ::Explorer::Log::GetCoreLogger()->trace(__VA_ARGS__)	//��ʾ��־
#define EXP_CORE_INFO(...) ::Explorer::Log::GetCoreLogger()->info(__VA_ARGS__)		//��Ϣ��־
#define EXP_CORE_WARN(...) ::Explorer::Log::GetCoreLogger()->warn(__VA_ARGS__)		//������־
#define EXP_CORE_ERROR(...) ::Explorer::Log::GetCoreLogger()->error(__VA_ARGS__)	//������־
#define EXP_CORE_FATAL(...) ::Explorer::Log::GetCoreLogger()->fatal(__VA_ARGS__)	//���ش�����־

//�ͻ�����־
#define EXP_TRACE(...) ::Explorer::Log::GetClientLogger()->trace(__VA_ARGS__)	//��ʾ��־
#define EXP_INFO(...) ::Explorer::Log::GetClientLogger()->info(__VA_ARGS__)	//��Ϣ��־
#define EXP_WARN(...) ::Explorer::Log::GetClientLogger()->warn(__VA_ARGS__)	//������־
#define EXP_ERROR(...) ::Explorer::Log::GetClientLogger()->error(__VA_ARGS__)	//������־
#define EXP_FATAL(...) ::Explorer::Log::GetClientLogger()->fatal(__VA_ARGS__)	//���ش�����־