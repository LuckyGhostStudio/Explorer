#pragma once

#include "Explorer/Core/UUID.h"

namespace Explorer
{
	/// <summary>
	/// ID���
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
	/// �������������Object�Ĺ�������
	/// </summary>
	class Self
	{
	private:
		std::string m_ObjectName;	//���ش������������
		bool m_ObjectEnable;		//�Ƿ���������

		//std::vector<entt::entity> m_ChildrenID{ entt::null };	//������ID TODO:��Ӹ�������
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