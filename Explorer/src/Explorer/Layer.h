#pragma once

#include "Explorer/Core.h"
#include "Explorer/Events/Event.h"

namespace Explorer
{
	/// <summary>
	/// 层
	/// </summary>
	class EXPLORER_API Layer
	{
	protected:
		std::string m_DebugName;	//Layer在Debug模式中的名字
	public:
		Layer(const std::string& name = "Layer");
		virtual ~Layer();

		/// <summary>
		/// 将该层固定到层栈
		/// </summary>
		virtual void OnAttach() {}

		/// <summary>
		/// 将该层从层栈分离
		/// </summary>
		virtual void OnDetach() {}

		/// <summary>
		/// 该层更新时调用：每帧调用
		/// </summary>
		virtual void OnUpdate() {}

		/// <summary>
		/// 事件函数：接收事件
		/// </summary>
		/// <param name="event">事件</param>
		virtual void OnEvent(Event& event) {}

		inline const std::string& GetName() const { return m_DebugName; }
	};
}