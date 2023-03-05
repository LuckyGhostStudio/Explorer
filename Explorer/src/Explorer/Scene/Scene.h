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
		friend class SceneHierarchyPanel;

		entt::registry m_Registry;	//实体注册表：实体id集合（unsigned int集合）
		std::string m_Name;			//场景名

		uint32_t m_ViewportWidth = 0;	//场景视口宽
		uint32_t m_ViewportHeight = 0;	//场景视口高
	public:
		Scene(const std::string& name = "New Scene");
		~Scene();

		inline const std::string& GetName() const { return m_Name; }
		inline void SetName(const std::string& name) { m_Name = name; }

		/// <summary>
		/// 创建物体
		/// </summary>
		/// <param name="name">物体名</param>
		/// <returns>物体</returns>
		Object CreateObject(const std::string& name = "Object");

		/// <summary>
		/// 更新：每帧调用
		/// </summary>
		/// <param name="dt">帧间隔</param>
		void OnUpdate(DeltaTime dt);

		/// <summary>
		/// 重置视口大小：视口改变时调用
		/// </summary>
		/// <param name="width">宽</param>
		/// <param name="height">高</param>
		void OnViewportResize(uint32_t width, uint32_t height);
	};
}