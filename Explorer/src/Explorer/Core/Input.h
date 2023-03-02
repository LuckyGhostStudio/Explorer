#pragma once

#include "Explorer/Core/Core.h"

namespace Explorer
{
	/// <summary>
	/// 输入类
	/// </summary>
	class Input
	{
	public:
		/// <summary>
		/// 按键是否按下
		/// </summary>
		/// <param name="keycode">按键代码</param>
		/// <returns>结果</returns>
		static bool IsKeyPressed(int keycode);

		/// <summary>
		/// 鼠标按钮是否按下
		/// </summary>
		/// <param name="button">按钮代码</param>
		/// <returns>结果</returns>
		static bool IsMouseButtonPressed(int button);

		/// <summary>
		/// 返回鼠标位置坐标
		/// </summary>
		/// <returns>鼠标位置(x, y)</returns>
		static std::pair<float, float> GetMousePosition();

		/// <summary>
		/// 返回鼠标x坐标
		/// </summary>
		/// <returns>鼠标x坐标</returns>
		static float GetMouseX();

		/// <summary>
		/// 返回鼠标y坐标
		/// </summary>
		/// <returns>鼠标y坐标</returns>
		static float GetMouseY();
	};
}