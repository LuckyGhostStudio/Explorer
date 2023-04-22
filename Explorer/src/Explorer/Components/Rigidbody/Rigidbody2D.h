#pragma once

#include "Explorer/Components/Component.h"

namespace Explorer
{
	/// <summary>
	/// 2D刚体
	/// </summary>
	class Rigidbody2D :public Component
	{
	public:
		/// <summary>
		/// 刚体类型
		/// </summary>
		enum class BodyType
		{
			Dynamic = 0,	//动态
			Static = 1,		//静态：静止的
			Kinematic = 2	//动力学的：不受重力影响
		};
	private:
		BodyType m_Type = BodyType::Dynamic;	//刚体类型
		bool m_FreezeRotation = false;			//冻结旋转 z轴
		void* m_RuntimeBody = nullptr;			//运行时刚体：指向运行时的自己
	private:
		virtual void SetName() override { m_Name = "Rigidbody 2D"; }

		/// <summary>
		/// 设置Transform组件图标
		/// </summary>
		virtual void SetIcon() override { m_Icon = std::make_shared<Texture2D>("Resources/Icons/Components/Rigidbody_Icon.png"); }
	public:
		Rigidbody2D() = default;
		Rigidbody2D(const Rigidbody2D&) = default;

		virtual const std::string& GetName() override { SetName(); return m_Name; }
		virtual std::shared_ptr<Texture2D>& GetIcon() override { SetIcon(); return m_Icon; }

		virtual bool GetSelectableEnable() override
		{
			m_SelectableEnable = false;		//禁用勾选框
			return m_SelectableEnable;
		}

		BodyType GetBodyType() const { return m_Type; }
		void SetBodyType(BodyType type) { m_Type = type; }

		bool GetFreezeRotation() const { return m_FreezeRotation; }
		bool& GetFreezeRotation_Ref() { return m_FreezeRotation; }
		void SetFreezeRotation(bool freeze) { m_FreezeRotation = freeze; }

		void* GetRuntimeBody() { return m_RuntimeBody; }
		void SetRuntimeBody(void* runtimeBody) { m_RuntimeBody = runtimeBody; }
	};
}