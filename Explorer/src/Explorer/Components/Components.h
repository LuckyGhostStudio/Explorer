#pragma once

#include "Explorer/Scene/ScriptableObject.h"

namespace Explorer
{
	/// <summary>
	/// �������������Object�Ĺ�������
	/// </summary>
	class Self
	{
	private:
		std::string m_ObjectName;	//���ش������������
		bool m_ObjectEnable;		//�Ƿ���������

		std::vector<entt::entity> m_ChildrenID{ entt::null };	//������ID TODO:��Ӹ�������
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

	/// <summary>
	/// ���ؽű����
	/// </summary>
	class NativeScript
	{
	public:
		ScriptableObject* Instance = nullptr;		//�ɽű���ʵ��ʵ�������ڵ��ýű��ڵĺ���

		ScriptableObject* (*InstantiateScript)();	//ʵ�����ű�����������ʵ����ScriptableEntity�ű�
		void (*DestroyScript)(NativeScript*);		//���ٽű���������������ScriptableEntity�ű���ʵ��

		/// <summary>
		/// ��T���͵Ľű����ű����
		/// </summary>
		/// <typeparam name="T">�ű�����</typeparam>
		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableObject*>(new T()); };	//ʵ�����ű���
			DestroyScript = [](NativeScript* script) { delete script->Instance; script->Instance = nullptr; };	//���ٽű����ʵ��
		}
	};
}