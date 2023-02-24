#include "exppch.h"	//Ԥ����ͷ�ļ�
#include "Log.h"
#include <spdlog/sinks/stdout_color_sinks.h>

namespace Explorer
{
	std::shared_ptr<spdlog::logger> Log::s_CoreLogger;		//�ں���־
	std::shared_ptr<spdlog::logger> Log::s_ClientLogger;		//�ͻ�����־

	void Log::Init()
	{
		spdlog::set_pattern("%^[%T] %n: %v%$");		//��־��ʽ��ʱ�� ���� ʵ����Ϣ

		s_CoreLogger = spdlog::stdout_color_mt("EXPLORER");
		s_CoreLogger->set_level(spdlog::level::trace);		//���ò㼶

		s_ClientLogger = spdlog::stdout_color_mt("APP");
		s_ClientLogger->set_level(spdlog::level::trace);	//���ò㼶
	}
}