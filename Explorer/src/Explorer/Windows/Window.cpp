#include "exppch.h"
#include "Window.h"

namespace Explorer
{
	static bool s_GLFWInitialized = false;	//GLFW�Ƿ��ѳ�ʼ��

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

		EXP_CORE_INFO("Cteating window {0} ({1}, {2})", props.Title, props.Width, props.Height);	//�����־��Ϣ����������

		if (!s_GLFWInitialized) {		//δ��ʼ��
			// TODO: glfwTerminate on system shutdown
			int success = glfwInit();	//��ʼ��GLFW����
			EXP_CORE_ASSERT(success, "Could not initialize GLFW!");	//��ʼ��ʧ�� ���ܳ�ʼ��GLFW

			s_GLFWInitialized = true;
		}

		//����GLFW����
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(m_Window);				//���ô���������Ϊ��ǰ�߳���������
		glfwSetWindowUserPointer(m_Window, &m_Data);	//������m_Data���ݸ�m_Window
		SetVSync(true);									//��ֱͬ��
	}

	void Window::Shutdown()
	{
		glfwDestroyWindow(m_Window);	//���ٴ���
	}
	
	void Window::OnUpdate()
	{
		glfwPollEvents();			//��ѯ���д�������¼�
		glfwSwapBuffers(m_Window);	//����ǰ�󻺳���
	}

	void Window::SetVSync(bool enabled)
	{
		if (enabled) {
			glfwSwapInterval(1);	//�������Ϊ1֡
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