#include "exppch.h"
#include "Input.h"

#include "Explorer/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Explorer
{
	bool Input::IsKeyPressed(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());	//GLFW����
		auto state = glfwGetKey(window, keycode);				//��ȡkeycode����״̬
		return state == GLFW_PRESS || state == GLFW_REPEAT;		//��������||�����ظ�
	}
	bool Input::IsMouseButtonPressed(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());	//GLFW����
		auto state = glfwGetMouseButton(window, button);		//��ȡbutton״̬
		return state == GLFW_PRESS;								//��������
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());	//GLFW����

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);		//��ȡ��� x y ����

		return { (float)xPos, (float)yPos };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().first;	//x����
	}
	float Input::GetMouseY()
	{
		return GetMousePosition().second;	//y����
	}
}