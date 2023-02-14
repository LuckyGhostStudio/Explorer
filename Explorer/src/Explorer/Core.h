#pragma once

#ifdef EXP_PLATFORM_WINDOWS		//windowsƽ̨
#if EXP_DYNAMIC_LINK	//��̬����
	#ifdef EXP_BUILD_DLL		//Explorer dll������ʱ����Hazel�У�
		#define EXPLORER_API __declspec(dllexport)		//����Explorer dll
	#else	//��Sandbox��
		#define EXPLORER_API __declspec(dllimport)		//����Explorer dll
	#endif
#else
	#define EXPLORER_API
#endif
#else
	#error Explorer only supports Windows!
#endif

#ifdef EXP_DEBUG
	#define EXP_ENABLE_ASSERTS
#endif // HZ_DEBUG

#ifdef EXP_ENABLE_ASSERTS	//���ö���
	//���ԣ�xΪ�� ����ʾERROR��־��Ϣ ���жϵ���
	#define EXP_ASSERT(x, ...) { if(!(x)) { EXP_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	//���ԣ�xΪ�� ����ʾERROR��־��Ϣ ���жϵ���
	#define EXP_CORE_ASSERT(x, ...) { if(!(x)) { EXP_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else	//���ö���
	#define EXP_ASSERT(x, ...)
	#define EXP_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)		//1����xλ

#define EXP_BIND_EVENT_FUNC(func) std::bind(&func, this, std::placeholders::_1)	//���¼����� ���غ�������