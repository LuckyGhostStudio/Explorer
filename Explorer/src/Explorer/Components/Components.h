#pragma once

#include "Explorer/Core/UUID.h"

namespace Explorer
{
	/// <summary>
	/// ID组件
	/// </summary>
	class ID
	{
	private:
		UUID m_ID;	//ID
	public:
		ID() = default;
		ID(const ID&) = default;
		ID(const UUID& uuid) :m_ID(uuid) {};

		UUID GetID() const { return m_ID; }
		void SetID(const UUID& id) { m_ID = id; }
	};

	/// <summary>
	/// 自身属性组件：Object的固有属性
	/// </summary>
	class Self
	{
	private:
		std::string m_ObjectName;	//挂载此组件的物体名
		bool m_ObjectEnable;		//是否启用物体

		//std::vector<entt::entity> m_ChildrenID{ entt::null };	//子物体ID TODO:添加父子物体
	public:
		Self() = default;
		Self(const Self&) = default;
		Self(const std::string& objectName, bool enableObject = true) 
			:m_ObjectName(objectName), m_ObjectEnable(enableObject) {}

		std::string& GetObjectName() { return m_ObjectName; }
		void SetObjectName(const std::string& name) { m_ObjectName = name; }

		bool GetObjectEnable() const { return m_ObjectEnable; }
		bool& GetObjectEnable_Ref() { return m_ObjectEnable; }
		void SetObjectEnable(bool enable) { m_ObjectEnable = enable; }
	};
}