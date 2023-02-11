#pragma once

#include "Core.h"

#include "Windows/Window.h"
#include "Explorer/LayerStack.h"
#include "Explorer/Events/Event.h"
#include "Explorer/Events/ApplicationEvent.h"

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
		LayerStack m_LayerStack;	

		/// <summary>
		/// 窗口关闭回调函数
		/// </summary>
		/// <param name="e">窗口关闭事件</param>
		/// <returns>是否已关闭</returns>
		bool OnWindowClose(WindowCloseEvent& e);
	public:
		Application();
		virtual ~Application();

		/// <summary>
		/// 事件回调函数
		/// </summary>
		/// <param name="e">事件</param>
		void OnEvent(Event& e);

		/// <summary>
		/// 运行
		/// </summary>
		void Run();

		/// <summary>
		/// 添加普通层到层栈
		/// </summary>
		/// <param name="layer">层</param>
		void PushLayer(Layer* layer);

		/// <summary>
		/// 添加覆盖层到层栈
		/// </summary>
		/// <param name="layer">层</param>
		void PushOverlay(Layer* layer);
	};

	/// <summary>
	/// 创建应用程序：在SandboxApp定义
	/// </summary>
	/// <returns>应用程序指针</returns>
	Application* CreateApplication();
}