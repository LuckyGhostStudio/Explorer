#pragma once

#ifdef EXP_DEBUG
	#define EXP_ENABLE_ASSERTS
#endif // HZ_DEBUG

#ifdef EXP_ENABLE_ASSERTS	//启用断言
	//断言：x为假 则显示ERROR日志信息 并中断调试
	#define EXP_ASSERT(x, ...) { if(!(x)) { EXP_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
	//断言：x为假 则显示ERROR日志信息 并中断调试
	#define EXP_CORE_ASSERT(x, ...) { if(!(x)) { EXP_CORE_ERROR("Assertion Failed: {0}", __VA_ARGS__); __debugbreak(); } }
#else	//禁用断言
	#define EXP_ASSERT(x, ...)
	#define EXP_CORE_ASSERT(x, ...)
#endif

#define BIT(x) (1 << x)		//1左移x位

#define EXP_BIND_EVENT_FUNC(func) std::bind(&func, this, std::placeholders::_1)	//绑定事件函数 返回函数对象