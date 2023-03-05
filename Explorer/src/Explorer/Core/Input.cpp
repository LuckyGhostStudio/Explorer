#include "exppch.h"
#include "Input.h"

#include "Explorer/Core/Application.h"
#include <GLFW/glfw3.h>

namespace Explorer
{
	bool Input::IsKeyPressed(KeyCode keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());	//GLFW窗口
		auto state = glfwGetKey(window, (int)keycode);				//获取keycode按键状态
		return state == GLFW_PRESS || state == GLFW_REPEAT;		//按键按下||按键重复
	}
	bool Input::IsMouseButtonPressed(MouseCode button)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());	//GLFW窗口
		auto state = glfwGetMouseButton(window, (int)button);		//获取button状态
		return state == GLFW_PRESS;								//按键按下
	}

	std::pair<float, float> Input::GetMousePosition()
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());	//GLFW窗口

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);		//获取鼠标 x y 坐标

		return { (float)xPos, (float)yPos };
	}

	float Input::GetMouseX()
	{
		return GetMousePosition().first;	//x坐标
	}
	float Input::GetMouseY()
	{
		return GetMousePosition().second;	//y坐标
	}
}