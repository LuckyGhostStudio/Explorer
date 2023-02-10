#pragma once

#include "Core.h"
#include "Events/Event.h"

#include "Windows/Window.h"

namespace Explorer
{
	/// <summary>
	/// 应用程序类：引擎入口
	/// </summary>
	class EXPLORER_API Application
	{
	private:
		std::unique_ptr<Window> m_Window;	//窗口指针
		bool m_Running = true;				//是否正在运行
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