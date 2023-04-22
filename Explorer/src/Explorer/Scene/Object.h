#pragma once

#include "Scene.h"

#include "entt.hpp"

#include "Explorer/Components/Components.h"

namespace Explorer
{
	/// <summary>
	/// 物体
	/// </summary>
	class Object
	{
	private:
		entt::entity m_ObjectID{ entt::null };	//实体ID
		Scene* m_Scene = nullptr;				//实体所属场景
		//TODO:添加父子物体
	public:
		Object() {}
		Object(entt::entity objectID, Scene* scene);
		Object(const Object& other) = default;

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
			T& component = m_Scene->m_Registry.emplace<T>(m_ObjectID, std::forward<Args>(args)...);	//向m_Scene场景的实体注册表添加T类型组件
			m_Scene->OnComponentAdded<T>(*this, component);											//m_Scene向this物体添加T组件时调用
			return component;
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
		operator entt::entity() const { return m_ObjectID; }
		operator uint32_t() const { return (uint32_t)m_ObjectID; }

		UUID GetUUID() { return GetComponent<ID>().GetID(); }

		bool operator==(const Object& other)
		{
			return m_ObjectID == other.m_ObjectID && m_Scene == other.m_Scene;	//物体id相同 && 所属场景相同
		}

		bool operator!=(const Object& other)
		{
			return !(*this == other);
		}
	};
}