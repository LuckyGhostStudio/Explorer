#include "exppch.h"	//预编译头文件
#include "Application.h"

#include "Explorer/Events/ApplicationEvent.h"
#include "Explorer/Log.h"

#include <GLFW/glfw3.h>

namespace Explorer
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)		//绑定函数x 返回函数对象

	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());	//创建窗口
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));		//设置回调函数
	}

	Application::~Application()
	{

	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);	//事件调度器
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));	//调度 窗口关闭事件

		EXP_CORE_TRACE("{0}", e);	//事件触发日志
	}

	void Application::Run()
	{
		while (m_Running) {
			glClearColor(0.3, 0.5, 0.8, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Window->OnUpdate();		//在OnUpdate中轮询处理接收的事件
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;	//结束运行
		return true;
	}
}