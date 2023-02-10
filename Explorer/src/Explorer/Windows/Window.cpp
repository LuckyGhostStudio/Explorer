#include "exppch.h"
#include "Window.h"

namespace Explorer
{
	static bool s_GLFWInitialized = false;	//GLFW是否已初始化

	Window* Window::Create(const WindowProps& props)
	{
		return new Window(props);
	}

	Window::Window(const WindowProps& props)
	{
		Init(props);
	}

	Window::~Window()
	{
		Shutdown();
	}
	
	void Window::Init(const WindowProps& props)
	{
		m_Data.Title = props.Title;
		m_Data.Width = props.Width;
		m_Data.Height = props.Height;

		EXP_CORE_INFO("Cteating window {0} ({1}, {2})", props.Title, props.Width, props.Height);	//输出日志信息：创建窗口

		if (!s_GLFWInitialized) {		//未初始化
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();	//初始化GLFW窗口
			EXP_CORE_ASSERT(success, "Could not initialize GLFW!");	//初始化失败 不能初始化GLFW

			s_GLFWInitialized = true;
		}

		//创建GLFW窗口
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(m_Window);				//设置窗口上下文为当前线程主上下文
		glfwSetWindowUserPointer(m_Window, &m_Data);	//将数据m_Data传递给m_Window
		SetVSync(true);									//垂直同步
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);	//销毁窗口
	}
	
	void Window::OnUpdate()
	{
		glfwPollEvents();			//轮询所有待处理的事件
		glfwSwapBuffers(m_Window);	//交换前后缓冲区
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled) {
			glfwSwapInterval(1);	//交换间隔为1帧
		}
		else {
			glfwSwapInterval(0);
		}

		m_Data.VSync = enabled;
	}
	
	bool Window::IsVSync() const
	{
		return m_Data.VSync;
	}
}