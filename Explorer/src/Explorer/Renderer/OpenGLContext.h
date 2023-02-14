#pragma once

struct GLFWwindow;

namespace Explorer
{
	/// <summary>
	/// OpenGL������
	/// </summary>
	class OpenGLContext
	{
	private:
		GLFWwindow* m_WindowHandle;		//���ھ��

	public:
		/// <summary>
		/// OpenGL������
		/// </summary>
		/// <param name="windowHandle">���ھ��</param>
		OpenGLContext(GLFWwindow* windowHandle);

		/// <summary>
		/// ��ʼ��
		/// </summary>
		void Init();

		/// <summary>
		/// ����ǰ�󻺳�
		/// </summary>
		void SwapBuffers();
	};
}