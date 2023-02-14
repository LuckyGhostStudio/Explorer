#pragma once

struct GLFWwindow;

namespace Explorer
{
	/// <summary>
	/// OpenGL上下文
	/// </summary>
	class OpenGLContext
	{
	private:
		GLFWwindow* m_WindowHandle;		//窗口句柄

	public:
		/// <summary>
		/// OpenGL上下文
		/// </summary>
		/// <param name="windowHandle">窗口句柄</param>
		OpenGLContext(GLFWwindow* windowHandle);

		/// <summary>
		/// 初始化
		/// </summary>
		void Init();

		/// <summary>
		/// 交换前后缓冲
		/// </summary>
		void SwapBuffers();
	};
}