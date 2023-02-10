#include "exppch.h"	//预编译头文件
#include "Application.h"

#include "Explorer/Events/ApplicationEvent.h"
#include "Explorer/Log.h"

#include <GLFW/glfw3.h>

namespace Explorer
{
	Application::Application()
	{
		m_Window = std::unique_ptr<Window>(Window::Create());	//创建窗口
	}

	Application::~Application()
	{

	}

	void Application::Run()
	{
		while (m_Running) {
			glClearColor(0.3, 0.5, 0.8, 1.0);
			glClear(GL_COLOR_BUFFER_BIT);
			m_Window->OnUpdate();
		}
	}
}