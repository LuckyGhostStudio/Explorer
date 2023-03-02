#pragma once

#include "entt.hpp"

#include "Explorer/Core/DeltaTime.h"

namespace Explorer
{
	/// <summary>
	/// ����
	/// </summary>
	class Scene
	{
	private:
		entt::registry m_Registry;	//ʵ�弯�ϣ�ʵ��id���ϣ�unsigned int���ϣ�
	public:
		Scene();
		~Scene();

		entt::registry& Reg() { return m_Registry; }

		/// <summary>
		/// ����ʵ��
		/// </summary>
		/// <returns>ʵ��</returns>
		entt::entity CreateEntity();

		/// <summary>
		/// ���£�ÿ֡����
		/// </summary>
		/// <param name="dt">֡���</param>
		void OnUpdate(DeltaTime dt);
	};
}