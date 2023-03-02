#pragma once

#include "entt.hpp"

#include "Explorer/Core/DeltaTime.h"

namespace Explorer
{
	/// <summary>
	/// 场景
	/// </summary>
	class Scene
	{
	private:
		entt::registry m_Registry;	//实体集合：实体id集合（unsigned int集合）
	public:
		Scene();
		~Scene();

		entt::registry& Reg() { return m_Registry; }

		/// <summary>
		/// 创建实体
		/// </summary>
		/// <returns>实体</returns>
		entt::entity CreateEntity();

		/// <summary>
		/// 更新：每帧调用
		/// </summary>
		/// <param name="dt">帧间隔</param>
		void OnUpdate(DeltaTime dt);
	};
}