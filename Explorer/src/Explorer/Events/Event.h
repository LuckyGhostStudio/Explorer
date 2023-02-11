#pragma once
#include "exppch.h"	//预编译头文件

namespace Explorer
{
	/*
	* Events in Explorer are currently blocking,meaning when an event occurs
	* itimmediately gets dispatched and must be dealt with right then an there.
	* For the future, a better strategy might be to buffer events in an event
	* bus and process them during the "event" part of the update stage.
	*
	* Explorer中的事件当前处于阻塞状态，这意味着事件发生时
	* 它立即被发送，必须立即处理。
	* 未来，更好的策略可能是在事件中缓冲事件
	* 总线，并在更新阶段的“事件”部分处理它们。
	*/

	/// <summary>
	/// 事件类型
	/// </summary>
	enum class EventType
	{
		None = 0,

		WindowClose,			//窗口关闭事件
		WindowResize,			//窗口大小改变事件
		WindowFocus,			//窗口聚焦事件
		WindowLostFocus,		//窗口失焦事件
		WindowMoved,			//窗口移动事件

		AppTick,				//App时钟事件
		AppUpdate,				//App每帧更新事件
		AppRender,				//App渲染事件

		KeyPressed,				//按键按下事件
		KeyReleased,			//按键抬起事件
		keyTyped,				//按键输入字符事件

		MouseButtonPressed,		//鼠标按钮按下事件
		MouseButtonReleased,	//鼠标按钮抬起事件
		MouseMoved, 			//鼠标移动事件
		MouseScrolled			//鼠标滚轮滚动事件
	};

	/// <summary>
	/// 事件种类
	/// </summary>
	enum EventCategory
	{
		None = 0,
		EventCategoryApplication = BIT(0),	// 1  App类事件
		EventCategoryInput = BIT(1),		// 2  输入类事件
		EventCategoryKeyboard = BIT(2),		// 4  键盘类事件
		EventCategoryMouse = BIT(3),		// 8  鼠标类事件
		EventCategoryMouseButton = BIT(4)	// 16 鼠标按钮类事件
	};

	/// <summary>
	/// 事件类
	/// </summary>
	class EXPLORER_API Event
	{
		friend class EventDispatcher;	//事件调度器
	public:
		/// <summary>
		/// 返回事件类型
		/// </summary>
		/// <returns>事件类型</returns>
		virtual EventType GetEventType() const = 0;

		/// <summary>
		/// 返回事件名
		/// </summary>
		/// <returns>事件名</returns>
		virtual const char* GetName() const = 0;

		/// <summary>
		/// 返回事件类别标志
		/// </summary>
		/// <returns>事件类别标志</returns>
		virtual int GetCategoryFlags() const = 0;

		/// <summary>
		/// 返回事件名：字符串
		/// </summary>
		/// <returns>事件名</returns>
		virtual std::string ToString() const { return GetName(); }

		/// <summary>
		/// 当前事件是否在目标事件分类中
		/// </summary>
		/// <param name="category">目标事件分类</param>
		/// <returns>结果</returns>
		inline bool IsInCategory(EventCategory category)
		{
			return GetCategoryFlags() & category;	//事件类别标志和 目标事件类别 按位与
		}

		/// <summary>
		/// 获取事件处理结果
		/// </summary>
		/// <returns></returns>
		inline bool GetHandled() const { return m_Handled; }
	protected:
		bool m_Handled = false;		//是否已处理
	};

	/// <summary>
	/// 事件调度器
	/// </summary>
	class EventDispatcher
	{
		template<typename T>
		using EventFn = std::function<bool(T&)>;	//事件函数 bool func(T&)
	private:
		Event& m_Event;		//当前接收的事件
	public:
		EventDispatcher(Event& event) :m_Event(event) {}

		/// <summary>
		/// 事件调度
		/// </summary>
		/// <typeparam name="T">类型</typeparam>
		/// <param name="func">事件函数</param>
		/// <returns>调度结果</returns>
		template<typename T>
		bool Dispatch(EventFn<T> func)
		{
			if (m_Event.GetEventType() == T::GetStaticType()) {		//接收到的事件类型 == 事件函数类型
				m_Event.m_Handled = func(*(T*)&m_Event);			//执行事件函数 func(Event& e)：将m_Event转换为T
				return true;
			}
			return false;
		}
	};

	/// <summary>
	/// 重载左移运算符：输出事件
	/// </summary>
	/// <param name="os">输出流对象</param>
	/// <param name="e">事件</param>
	/// <returns>输出事件</returns>
	inline std::ostream& operator<<(std::ostream& os, const Event& e)
	{
		return os << e.ToString();
	}
}