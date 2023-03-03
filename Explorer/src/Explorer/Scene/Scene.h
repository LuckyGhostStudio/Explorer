#pragma once

#include "entt.hpp"

#include "Explorer/Core/DeltaTime.h"

namespace Explorer
{
	class Object;

	/// <summary>
	/// 场景
	/// </summary>
	class Scene
	{
	private:
		friend class Object;	//友元类Object

		entt::registry m_Registry;	//实体注册表：实体id集合（unsigned int集合）
		std::string m_Name;			//场景名
	public:
		Scene(const std::string& name = "New Scene");
		~Scene();

		inline const std::string& GetName() const { return m_Name; }
		inline void SetName(const std::string& name) { m_Name = name; }

		/// <summary>
		/// 创建实体
		/// </summary>
		/// <param name="name">实体名</param>
		/// <returns>实体</returns>
		Object CreateEntity(const std::string& name = std::string());

		/// <summary>
		/// 更新：每帧调用
		/// </summary>
		/// <param name="dt">帧间隔</param>
		void OnUpdate(DeltaTime dt);
	};
}