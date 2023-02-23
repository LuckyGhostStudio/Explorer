#pragma once

#include "Explorer/Core/Core.h"

namespace Explorer
{
	/// <summary>
	/// ������
	/// </summary>
	class EXPLORER_API Input
	{
	private:
		static Input* Instance;	//��̬ʵ��
	public:
		/// <summary>
		/// �����Ƿ���
		/// </summary>
		/// <param name="keycode">��������</param>
		/// <returns>���</returns>
		inline static bool IsKeyPressed(int keycode) { return Instance->IsKeyPressedImpl(keycode); }

		/// <summary>
		/// ��갴ť�Ƿ���
		/// </summary>
		/// <param name="button">��ť����</param>
		/// <returns>���</returns>
		inline static bool IsMouseButtonPressed(int button) { return Instance->IsMouseButtonPressedImpl(button); }

		/// <summary>
		/// �������λ������
		/// </summary>
		/// <returns>���λ��(x, y)</returns>
		inline static std::pair<float, float> GetMousePosition() { return Instance->GetMousePositionImpl(); }

		/// <summary>
		/// �������x����
		/// </summary>
		/// <returns>���x����</returns>
		inline static float GetMouseX() { return Instance->GetMouseXImpl(); }

		/// <summary>
		/// �������y����
		/// </summary>
		/// <returns>���y����</returns>
		inline static float GetMouseY() { return Instance->GetMouseYImpl(); }
	protected:
		/// <summary>
		/// �����Ƿ��£�����ʵ��
		/// </summary>
		/// <param name="keycode">��������</param>
		/// <returns>���</returns>
		virtual bool IsKeyPressedImpl(int keycode);

		/// <summary>
		/// ��갴ť�Ƿ��£�����ʵ��
		/// </summary>
		/// <param name="button">��ť����</param>
		/// <returns>���</returns>
		virtual bool IsMouseButtonPressedImpl(int button);

		/// <summary>
		/// �������λ�����꣺����ʵ��
		/// </summary>
		/// <returns>���λ��(x, y)</returns>
		virtual std::pair<float, float> GetMousePositionImpl();

		/// <summary>
		/// �������x���꣺����ʵ��
		/// </summary>
		/// <returns>���x����</returns>
		virtual float GetMouseXImpl();

		/// <summary>
		/// �������y���꣺����ʵ��
		/// </summary>
		/// <returns>���y����</returns>
		virtual float GetMouseYImpl();
	};
}