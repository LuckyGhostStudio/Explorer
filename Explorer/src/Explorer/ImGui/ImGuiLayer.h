#pragma once

#include "Explorer/Layer.h"

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
	};
}