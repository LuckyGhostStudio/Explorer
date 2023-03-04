#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace Explorer
{
	/// <summary>
	/// ʵ��
	/// </summary>
	class Object
	{
	private:
		entt::entity m_ObjectID{ entt::null };	//ʵ��ID
		Scene* m_Scene = nullptr;		//ʵ����������

		std::string m_Name;
	public:
		Object() {}
		Object(entt::entity objectID, Scene* scene, const std::string& name = "Object");
		Object(const Object& other) = default;

		inline const std::string& GetName() const { return m_Name; }
		inline void SetName(const std::string& name) { m_Name = name; }

		/// <summary>
		/// ���T�������
		/// </summary>
		/// <typeparam name="T">�������</typeparam>
		/// <typeparam name="...Args">�����������</typeparam>
		/// <param name="...args">�������</param>
		/// <returns>���</returns>
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			EXP_CORE_ASSERT(!HasComponent<T>(), "Object already has component!");	//������Ѵ���

			return m_Scene->m_Registry.emplace<T>(m_ObjectID, std::forward<Args>(args)...);	//��m_Scene������ʵ��ע������T�������
		}

		/// <summary>
		/// ����T�������
		/// </summary>
		/// <typeparam name="T">�������</typeparam>
		/// <returns>���</returns>
		template<typename T>
		T& GetComponent()
		{
			EXP_CORE_ASSERT(HasComponent<T>(), "Object dose not have component!");	//�����������

			return m_Scene->m_Registry.get<T>(m_ObjectID);	//��m_Scene������ʵ��ע�����T�������
		}

		/// <summary>
		/// ��ѯ�Ƿ�ӵ��T�������
		/// </summary>
		/// <typeparam name="T">�������</typeparam>
		/// <returns>��ѯ���</returns>
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_ObjectID);	//����m_Scene������m_ObjectID��T�������
		}

		/// <summary>
		/// �Ƴ�T�������
		/// </summary>
		/// <typeparam name="T">�������</typeparam>
		template<typename T>
		void RemoveComponent()
		{
			EXP_CORE_ASSERT(HasComponent<T>(), "Object dose not have component!");	//�����������

			m_Scene->m_Registry.remove<T>(m_ObjectID);	//�Ƴ�m_Scene������m_ObjectHandle��T�������
		}

		operator bool() const { return m_ObjectID != entt::null; }
	};
}