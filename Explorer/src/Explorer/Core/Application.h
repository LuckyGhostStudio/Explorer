#pragma once

#include "Core.h"

#include "Explorer/Core/Window.h"
#include "Explorer/Core/LayerStack.h"
#include "Explorer/Events/Event.h"
#include "Explorer/Events/ApplicationEvent.h"

#include "Explorer/Core/DeltaTime.h"

#include "Explorer/ImGui/ImGuiLayer.h"

namespace Explorer
{
	/// <summary>
	/// 应用程序类：引擎入口
	/// </summary>
	class Application
	{
	private:
		static Application* Instance;		//静态实例

		std::unique_ptr<Window> m_Window;	//窗口指针
		ImGuiLayer* m_ImGuiLayer;			//ImGui层指针

		bool m_Running = true;				//是否正在运行
		bool m_Minimized = false;			//是否最小化
		LayerStack m_LayerStack;			//层栈

		float m_LastFrameTime = 0.0f;		//上一帧时间

		/// <summary>
		/// 窗口关闭回调函数
		/// </summary>
		/// <param name="e">窗口关闭事件</param>
		/// <returns>是否已关闭</returns>
		bool OnWindowClose(WindowCloseEvent& e);

		/// <summary>
		/// 窗口缩放时调用
		/// </summary>
		/// <param name="e">窗口缩放事件</param>
		/// <returns>事件处理结果</returns>
		bool OnWindowResize(WindowResizeEvent& e);
	public:
		Application();
		virtual ~Application();

		/// <summary>
		/// 运行
		/// </summary>
		void Run();

		/// <summary>
		/// 关闭程序
		/// </summary>
		void Close();

		/// <summary>
		/// 事件回调函数
		/// </summary>
		/// <param name="e">事件</param>
		void OnEvent(Event& e);

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

		/// <summary>
		/// 返回Application的实例
		/// </summary>
		/// <returns>实例</returns>
		inline static Application& GetInstance() { return *Instance; }

		/// <summary>
		/// 返回Application的窗口
		/// </summary>
		/// <returns></returns>
		inline Window& GetWindow() { return *m_Window; }
	};

	/// <summary>
	/// 创建应用程序：在SandboxApp定义
	/// </summary>
	/// <returns>应用程序指针</returns>
	Application* CreateApplication();
}