#pragma once

#ifdef EXP_PLATFORM_WINDOWS		//windowsƽ̨
	#ifdef EXP_BUILD_DLL		//Explorer dll������ʱ����Hazel�У�
		#define EXPLORER_API __declspec(dllexport)		//����Explorer dll
	#else	//��Sandbox��
		#define EXPLORER_API __declspec(dllimport)		//����Explorer dll
	#endif
#else
	#error Explorer only supports Windows!
#endif