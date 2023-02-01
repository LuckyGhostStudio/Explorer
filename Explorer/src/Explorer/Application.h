#pragma once

#include "Core.h"

namespace Explorer
{
	/// <summary>
	/// 应用程序类：引擎入口
	/// </summary>
	class EXPLORER_API Application
	{
	public:
		Application();
		virtual ~Application();

		/// <summary>
		/// 运行
		/// </summary>
		void Run();
	};

	/// <summary>
	/// 创建应用程序：在SandboxApp定义
	/// </summary>
	/// <returns>应用程序指针</returns>
	Application* CreateApplication();
}