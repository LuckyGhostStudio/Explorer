#pragma once

#include "exppch.h"

#include "Explorer/Core.h"
#include "Explorer/Events/Event.h"

#include <GLFW/glfw3.h>

namespace Explorer
{
	/// <summary>
	/// ��������
	/// </summary>
	struct WindowProps
	{
		std::string Title;		//����
		unsigned int Width;		//��
		unsigned int Height;	//��

		WindowProps(const std::string& title = "Explorer Engine", unsigned int width = 1280, unsigned int height = 720)
			:Title(title), Width(width), Height(height) {}
	};

	/// <summary>
	/// ������
	/// </summary>
	class EXPLORER_API Window
	{
	public:
		using EventCallbackFn = std::function<void(Event&)>;	//�¼��ص����� void func(Event&)

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="props">��������</param>
		/// <returns>����</returns>
		static Window* Create(const WindowProps& props = WindowProps());

		/// <summary>
		/// ����
		/// </summary>
		/// <param name="props">��������</param>
		Window(const WindowProps& props);

		virtual ~Window();

		/// <summary>
		/// ���£�ÿ֡����
		/// </summary>
		void OnUpdate();

		inline unsigned int GetWidth() const  { return m_Data.Width; }
		inline unsigned int GetHeight() const  { return m_Data.Height; }

		/// <summary>
		/// �����¼��ص���������Application����
		/// </summary>
		/// <param name="callback">�ص�����</param>
		inline void SetEventCallback(const EventCallbackFn& callback)
		{
			m_Data.EventCallback = callback;
		}

		/// <summary>
		/// ���ô�ֱͬ��
		/// </summary>
		/// <param name="enabled">�Ƿ�����</param>
		void SetVSync(bool enabled);

		/// <summary>
		/// ����Ƿ�ֱͬ��
		/// </summary>
		/// <returns>���</returns>
		bool IsVSync() const;
	private:
		/// <summary>
		/// ��ʼ������
		/// </summary>
		/// <param name="props">��������</param>
		virtual void Init(const WindowProps& props);

		/// <summary>
		/// �رմ���
		/// </summary>
		virtual void Shutdown();
	private:
		GLFWwindow* m_Window;	//GLFW����

		/// <summary>
		/// ��������
		/// </summary>
		struct WindowData
		{
			std::string Title;		//����
			unsigned int Width;		//��
			unsigned int Height;	//��
			bool VSync;				//�Ƿ�ֱͬ��

			EventCallbackFn EventCallback;	//�¼��ص�����
		};

		WindowData m_Data;	//��������
	};
}