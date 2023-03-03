#pragma once

#include "entt.hpp"

#include "Explorer/Core/DeltaTime.h"

namespace Explorer
{
	class Object;

	/// <summary>
	/// ����
	/// </summary>
	class Scene
	{
	private:
		friend class Object;	//��Ԫ��Object

		entt::registry m_Registry;	//ʵ��ע���ʵ��id���ϣ�unsigned int���ϣ�
		std::string m_Name;			//������
	public:
		Scene(const std::string& name = "New Scene");
		~Scene();

		inline const std::string& GetName() const { return m_Name; }
		inline void SetName(const std::string& name) { m_Name = name; }

		/// <summary>
		/// ����ʵ��
		/// </summary>
		/// <param name="name">ʵ����</param>
		/// <returns>ʵ��</returns>
		Object CreateEntity(const std::string& name = std::string());

		/// <summary>
		/// ���£�ÿ֡����
		/// </summary>
		/// <param name="dt">֡���</param>
		void OnUpdate(DeltaTime dt);
	};
}