#pragma once

#include "Explorer/Layer.h"

#include "Explorer/Events/ApplicationEvent.h"
#include "Explorer/Events/KeyEvent.h"
#include "Explorer/Events/MouseEvent.h"

namespace Explorer
{
	/// <summary>
	/// ImGui层
	/// </summary>
	class EXPLORER_API ImGuiLayer :public Layer
	{
	private:
		float m_Time = 0.0f;	//当前帧时间
	public:
		ImGuiLayer();
		~ImGuiLayer();

		/// <summary>
		/// 将该层添加到层栈时调用
		/// </summary>
		void OnAttach();

		/// <summary>
		/// 将该层从层栈移除时调用
		/// </summary>
		void OnDetach();

		/// <summary>
		/// 该层更新时调用：每帧调用
		/// </summary>
		void OnUpdate();

		/// <summary>
		/// 事件函数：接收事件
		/// </summary>
		/// <param name="event">事件</param>
		void OnEvent(Event& event);

		/// <summary>
		/// 鼠标按钮按下
		/// </summary>
		/// <param name="event">鼠标按钮按下事件</param>
		bool OnMouseButtonPressedEvent(MouseButtonPressedEvent& event);

		/// <summary>
		/// 鼠标按钮抬起
		/// </summary>
		/// <param name="event">鼠标按钮抬起事件</param>
		bool OnMouseButtonReleasedEvent(MouseButtonReleasedEvent& event);

		/// <summary>
		/// 鼠标移动
		/// </summary>
		/// <param name="event">鼠标移动事件</param>
		bool OnMouseMovedEvent(MouseMovedEvent& event);

		/// <summary>
		/// 鼠标滚轮滚动
		/// </summary>
		/// <param name="event">鼠标滚轮滚动事件</param>
		bool OnMouseScrolledEvent(MouseScrolledEvent& event);

		/// <summary>
		/// 键盘按键按下
		/// </summary>
		/// <param name="event">键盘按键按下事件</param>
		bool OnKeyPressedEvent(KeyPressedEvent& event);

		/// <summary>
		/// 键盘按键抬起
		/// </summary>
		/// <param name="event">键盘按键抬起事件</param>
		bool OnKeyReleasedEvent(KeyReleasedEvent& event);

		/// <summary>
		/// 键盘输入按键字符
		/// </summary>
		/// <param name="event">按键输入字符事件</param>
		/// <returns></returns>
		bool OnKeyTypedEvent(KeyTypedEvent& event);

		/// <summary>
		/// 窗口大小改变
		/// </summary>
		/// <param name="event">窗口大小改变事件</param>
		bool OnWindowResizeEvent(WindowResizeEvent& event);
	};
}