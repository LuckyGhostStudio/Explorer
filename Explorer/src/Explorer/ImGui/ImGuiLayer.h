#pragma once

#include "Explorer/Core/Layer.h"

#include "Explorer/Events/ApplicationEvent.h"
#include "Explorer/Events/KeyEvent.h"
#include "Explorer/Events/MouseEvent.h"

namespace Explorer
{
	/// <summary>
	/// ImGui层
	/// </summary>
	class ImGuiLayer :public Layer
	{
	private:
		float m_Time = 0.0f;		//当前帧时间
		bool m_BlockEvents = true;	//是否阻止接收事件
	public:
		ImGuiLayer();
		~ImGuiLayer();

		/// <summary>
		/// 将该层添加到层栈时调用
		/// </summary>
		virtual void OnAttach() override;

		/// <summary>
		/// 将该层从层栈移除时调用
		/// </summary>
		virtual void OnDetach() override;

		/// <summary>
		/// 事件回调函数
		/// </summary>
		/// <param name="e">事件</param>
		virtual void OnEvent(Event& e) override;

		/// <summary>
		/// 开始窗口渲染
		/// </summary>
		void Begin();

		/// <summary>
		/// 结束窗口渲染
		/// </summary>
		void End();

		/// <summary>
		/// 阻止事件：阻止接收事件
		/// </summary>
		/// <param name="block">是否阻止</param>
		void BlockEvents(bool block) { m_BlockEvents = block; }

		/// <summary>
		/// 设置深色主题
		/// </summary>
		void SetDarkThemeColors();
	};
}