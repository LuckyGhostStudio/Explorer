#pragma once

#ifdef EXP_PLATFORM_WINDOWS		//windows平台
	#ifdef EXP_BUILD_DLL		//Explorer dll被构建时（在Hazel中）
		#define EXPLORER_API __declspec(dllexport)		//导出Explorer dll
	#else	//在Sandbox中
		#define EXPLORER_API __declspec(dllimport)		//导入Explorer dll
	#endif
#else
	#error Explorer only supports Windows!
#endif