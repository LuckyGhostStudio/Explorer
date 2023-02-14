#include "exppch.h"
#include "OpenGLContext.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Explorer
{
	OpenGLContext::OpenGLContext(GLFWwindow* windowHandle) :m_WindowHandle(windowHandle)
	{
		EXP_CORE_ASSERT(m_WindowHandle, "Window handle is null!");
	}

	void OpenGLContext::Init()
	{
		glfwMakeContextCurrent(m_WindowHandle);				//���ô���������Ϊ��ǰ�߳���������

		int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);	//��ʼ��GLAD
		EXP_CORE_ASSERT(status, "Failed to initialize Glad!");

		EXP_CORE_INFO("OpenGL Info");										//OpenGL��Ϣ
		EXP_CORE_INFO("    Vendor: {0}", (char*)glGetString(GL_VENDOR));	//������
		EXP_CORE_INFO("    Renderer: {0}", (char*)glGetString(GL_RENDERER));//GPU����
		EXP_CORE_INFO("    Version: {0}", (char*)glGetString(GL_VERSION));	//�汾
	}

	void OpenGLContext::SwapBuffers()
	{
		glfwSwapBuffers(m_WindowHandle);	//����ǰ�󻺳���
	}
}