#pragma once

#include "Core.h"

namespace Explorer
{
	/// <summary>
	/// Ӧ�ó����ࣺ�������
	/// </summary>
	class EXPLORER_API Application
	{
	public:
		Application();
		virtual ~Application();

		/// <summary>
		/// ����
		/// </summary>
		void Run();
	};

	/// <summary>
	/// ����Ӧ�ó�����SandboxApp����
	/// </summary>
	/// <returns>Ӧ�ó���ָ��</returns>
	Application* CreateApplication();
}