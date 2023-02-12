#include "exppch.h"
#include "Input.h"

#include "Explorer/Application.h"
#include <GLFW/glfw3.h>

namespace Explorer
{
	Input* Input::Instance = new Input();		//��̬ʵ��

	bool Input::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());	//GLFW����
		auto state = glfwGetKey(window, keycode);				//��ȡkeycode����״̬
		return state == GLFW_PRESS || state == GLFW_REPEAT;		//��������||�����ظ�
	}
	bool Input::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());	//GLFW����
		auto state = glfwGetMouseButton(window, button);		//��ȡbutton״̬
		return state == GLFW_PRESS;								//��������
	}

	std::pair<float, float> Input::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());	//GLFW����

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);		//��ȡ��� x y ����

		return { (float)xPos, (float)yPos };
	}

	float Input::GetMouseXImpl()
	{
		return GetMousePositionImpl().first;	//x����
	}
	float Input::GetMouseYImpl()
	{
		return GetMousePositionImpl().second;	//y����
	}
}