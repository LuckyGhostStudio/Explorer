#pragma once

#include "Explorer/Core/Core.h"

namespace Explorer
{
	/// <summary>
	/// 输入类
	/// </summary>
	class EXPLORER_API Input
	{
	private:
		static Input* Instance;	//静态实例
	public:
		/// <summary>
		/// 按键是否按下
		/// </summary>
		/// <param name="keycode">按键代码</param>
		/// <returns>结果</returns>
		inline static bool IsKeyPressed(int keycode) { return Instance->IsKeyPressedImpl(keycode); }

		/// <summary>
		/// 鼠标按钮是否按下
		/// </summary>
		/// <param name="button">按钮代码</param>
		/// <returns>结果</returns>
		inline static bool IsMouseButtonPressed(int button) { return Instance->IsMouseButtonPressedImpl(button); }

		/// <summary>
		/// 返回鼠标位置坐标
		/// </summary>
		/// <returns>鼠标位置(x, y)</returns>
		inline static std::pair<float, float> GetMousePosition() { return Instance->GetMousePositionImpl(); }

		/// <summary>
		/// 返回鼠标x坐标
		/// </summary>
		/// <returns>鼠标x坐标</returns>
		inline static float GetMouseX() { return Instance->GetMouseXImpl(); }

		/// <summary>
		/// 返回鼠标y坐标
		/// </summary>
		/// <returns>鼠标y坐标</returns>
		inline static float GetMouseY() { return Instance->GetMouseYImpl(); }
	protected:
		/// <summary>
		/// 按键是否按下：具体实现
		/// </summary>
		/// <param name="keycode">按键代码</param>
		/// <returns>结果</returns>
		virtual bool IsKeyPressedImpl(int keycode);

		/// <summary>
		/// 鼠标按钮是否按下：具体实现
		/// </summary>
		/// <param name="button">按钮代码</param>
		/// <returns>结果</returns>
		virtual bool IsMouseButtonPressedImpl(int button);

		/// <summary>
		/// 返回鼠标位置坐标：具体实现
		/// </summary>
		/// <returns>鼠标位置(x, y)</returns>
		virtual std::pair<float, float> GetMousePositionImpl();

		/// <summary>
		/// 返回鼠标x坐标：具体实现
		/// </summary>
		/// <returns>鼠标x坐标</returns>
		virtual float GetMouseXImpl();

		/// <summary>
		/// 返回鼠标y坐标：具体实现
		/// </summary>
		/// <returns>鼠标y坐标</returns>
		virtual float GetMouseYImpl();
	};
}