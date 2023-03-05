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
		friend class SceneHierarchyPanel;

		entt::registry m_Registry;	//ʵ��ע���ʵ��id���ϣ�unsigned int���ϣ�
		std::string m_Name;			//������

		uint32_t m_ViewportWidth = 0;	//�����ӿڿ�
		uint32_t m_ViewportHeight = 0;	//�����ӿڸ�
	public:
		Scene(const std::string& name = "New Scene");
		~Scene();

		inline const std::string& GetName() const { return m_Name; }
		inline void SetName(const std::string& name) { m_Name = name; }

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="name">������</param>
		/// <returns>����</returns>
		Object CreateObject(const std::string& name = "Object");

		/// <summary>
		/// ���£�ÿ֡����
		/// </summary>
		/// <param name="dt">֡���</param>
		void OnUpdate(DeltaTime dt);

		/// <summary>
		/// �����ӿڴ�С���ӿڸı�ʱ����
		/// </summary>
		/// <param name="width">��</param>
		/// <param name="height">��</param>
		void OnViewportResize(uint32_t width, uint32_t height);
	};
}