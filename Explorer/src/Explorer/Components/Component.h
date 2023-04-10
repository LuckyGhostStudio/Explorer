#pragma once

#include "Explorer/Renderer/Texture.h"

namespace Explorer
{
	/// <summary>
	/// 所有组件父类
	/// </summary>
	class Component
	{
	protected:
		std::string m_Name;					//组件名
		std::shared_ptr<Texture2D> m_Icon;	//组件图标
		bool m_Enable = true;				//是否启用组件
		bool m_SelectableEnable = true;		//是否可选择启用组件与否：为false 则组件不能选择是否启用（没有选择启用勾选框，总是启用）
		
		Component() = default;
		
		Component(const Component&) = default;
		Component(const std::string& name) :m_Name(name) {}

		virtual ~Component() = default;
		
		virtual void SetName() = 0;

		/// <summary>
		/// 设置Icon图片
		/// </summary>
		virtual void SetIcon() = 0;
	public:
		virtual const std::string& GetName() = 0;
		virtual std::shared_ptr<Texture2D>& GetIcon() = 0;

		bool GetEnable() const { return m_Enable; }
		bool& GetEnable_Ref() { return m_Enable; }
		void SetEnable(bool enable) { m_Enable = enable; }

		virtual bool GetSelectableEnable() { return m_SelectableEnable; }
	};
}