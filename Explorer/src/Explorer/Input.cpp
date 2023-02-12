#include "exppch.h"
#include "Input.h"

#include "Explorer/Application.h"
#include <GLFW/glfw3.h>

namespace Explorer
{
	Input* Input::Instance = new Input();		//静态实例

	bool Input::IsKeyPressedImpl(int keycode)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());	//GLFW窗口
		auto state = glfwGetKey(window, keycode);				//获取keycode按键状态
		return state == GLFW_PRESS || state == GLFW_REPEAT;		//按键按下||按键重复
	}
	bool Input::IsMouseButtonPressedImpl(int button)
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());	//GLFW窗口
		auto state = glfwGetMouseButton(window, button);		//获取button状态
		return state == GLFW_PRESS;								//按键按下
	}

	std::pair<float, float> Input::GetMousePositionImpl()
	{
		auto window = static_cast<GLFWwindow*>(Application::GetInstance().GetWindow().GetNativeWindow());	//GLFW窗口

		double xPos, yPos;
		glfwGetCursorPos(window, &xPos, &yPos);		//获取鼠标 x y 坐标

		return { (float)xPos, (float)yPos };
	}

	float Input::GetMouseXImpl()
	{
		return GetMousePositionImpl().first;	//x坐标
	}
	float Input::GetMouseYImpl()
	{
		return GetMousePositionImpl().second;	//y坐标
	}
}