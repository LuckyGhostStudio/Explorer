#pragma once

#include "Scene.h"

#include "entt.hpp"

namespace Explorer
{
	/// <summary>
	/// 实体
	/// </summary>
	class Object
	{
	private:
		entt::entity m_ObjectID{ entt::null };	//实体ID
		Scene* m_Scene = nullptr;		//实体所属场景

		std::string m_Name;
	public:
		Object() {}
		Object(entt::entity objectID, Scene* scene, const std::string& name = "Object");
		Object(const Object& other) = default;

		inline const std::string& GetName() const { return m_Name; }
		inline void SetName(const std::string& name) { m_Name = name; }

		/// <summary>
		/// 添加T类型组件
		/// </summary>
		/// <typeparam name="T">组件类型</typeparam>
		/// <typeparam name="...Args">组件参数类型</typeparam>
		/// <param name="...args">组件参数</param>
		/// <returns>组件</returns>
		template<typename T, typename... Args>
		T& AddComponent(Args&&... args)
		{
			EXP_CORE_ASSERT(!HasComponent<T>(), "Object already has component!");	//该组件已存在

			return m_Scene->m_Registry.emplace<T>(m_ObjectID, std::forward<Args>(args)...);	//向m_Scene场景的实体注册表添加T类型组件
		}

		/// <summary>
		/// 返回T类型组件
		/// </summary>
		/// <typeparam name="T">组件类型</typeparam>
		/// <returns>组件</returns>
		template<typename T>
		T& GetComponent()
		{
			EXP_CORE_ASSERT(HasComponent<T>(), "Object dose not have component!");	//该组件不存在

			return m_Scene->m_Registry.get<T>(m_ObjectID);	//从m_Scene场景的实体注册表获得T类型组件
		}

		/// <summary>
		/// 查询是否拥有T类型组件
		/// </summary>
		/// <typeparam name="T">组件类型</typeparam>
		/// <returns>查询结果</returns>
		template<typename T>
		bool HasComponent()
		{
			return m_Scene->m_Registry.has<T>(m_ObjectID);	//查找m_Scene场景中m_ObjectID的T类型组件
		}

		/// <summary>
		/// 移除T类型组件
		/// </summary>
		/// <typeparam name="T">组件类型</typeparam>
		template<typename T>
		void RemoveComponent()
		{
			EXP_CORE_ASSERT(HasComponent<T>(), "Object dose not have component!");	//该组件不存在

			m_Scene->m_Registry.remove<T>(m_ObjectID);	//移除m_Scene场景中m_ObjectHandle的T类型组件
		}

		operator bool() const { return m_ObjectID != entt::null; }
	};
}