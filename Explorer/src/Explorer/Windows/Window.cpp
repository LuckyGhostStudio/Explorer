#include "exppch.h"
#include "Window.h"

#include "Explorer/Events/ApplicationEvent.h"
#include "Explorer/Events/KeyEvent.h"
#include "Explorer/Events/MouseEvent.h"

#include "glad/glad.h"

namespace Explorer
{
	static bool s_GLFWInitialized = false;	//GLFW�Ƿ��ѳ�ʼ��

	/// <summary>
	/// GLFW����ص�����
	/// </summary>
	/// <param name="error">��������</param>
	/// <param name="description">��������</param>
	static void GLFWErrorCallback(int error, const char* description)
	{
		EXP_CORE_ERROR("GLFW Error ({0}): {1}", error, description);		//���������־
	}

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
			glfwSetErrorCallback(GLFWErrorCallback);	//���ô���ص�����

			s_GLFWInitialized = true;
		}

		//����GLFW����
		m_Window = glfwCreateWindow((int)props.Width, (int)props.Height, m_Data.Title.c_str(), nullptr, nullptr);

		glfwMakeContextCurrent(m_Window);				//���ô���������Ϊ��ǰ�߳���������

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);	//��ʼ��GLAD
		EXP_CORE_ASSERT(status, "Failed to initialize Glad!");

		glfwSetWindowUserPointer(m_Window, &m_Data);	//������m_Data���ݸ�m_Window��userPtrָ��m_Data
		SetVSync(true);									//��ֱͬ��

		//----����GLFW�Ļص�����----
		//���ڴ�С�ı�
		glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);	//ͨ��userPtrָ���ȡwindow���ڵ�data����

			//���ڸ��º��С
			data.Width = width;
			data.Height = height;

			WindowResizeEvent event(width, height);	//�������ڴ�С�ı��¼�
			data.EventCallback(event);				//�����¼��ص��������ص���������Application������ΪOnEvent��<=> OnEvent(event)
		});

		//���ڹر�
		glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			WindowCloseEvent event;		//���ڹر��¼�
			data.EventCallback(event);
		});

		//���̰������������� ����ɨ���� ��Ϊ �޸���λ��
		glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:	//��������
			{
				KeyPressedEvent event(key, 0);	//���������¼�
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:	//����̧��
			{
				KeyReleasedEvent event(key);	//����̧���¼�
				data.EventCallback(event);
				break;
			}
			case GLFW_REPEAT:	//�����ظ�
			{
				KeyPressedEvent event(key, 1);	//���������¼�
				data.EventCallback(event);
				break;
			}
			}
		});

		//���
		glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			switch (action)
			{
			case GLFW_PRESS:	//��������
			{
				MouseButtonPressedEvent event(button);	//��갴ť�����¼�
				data.EventCallback(event);
				break;
			}
			case GLFW_RELEASE:	//����̧��
			{
				MouseButtonReleasedEvent event(button);	//��갴ţ̌���¼�
				data.EventCallback(event);
				break;
			}
			}
		});

		//�����ֹ���
		glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xOffset, double yOffset)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseScrolledEvent event((float)xOffset, (float)yOffset);	//�����ֹ����¼�
			data.EventCallback(event);
		});

		//����ƶ�
		glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xPos, double yPos)
		{
			WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

			MouseMovedEvent event((float)xPos, (float)yPos);	//����ƶ��¼�
			data.EventCallback(event);
		});
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