#pragma once

#include "Explorer/Scene/Scene.h"
#include "Explorer/Core/Core.h"
#include "Explorer/Core/Log.h"
#include "Explorer/Scene/Object.h"

namespace Explorer
{
	/// <summary>
	/// ��������㼶���
	/// </summary>
	class SceneHierarchyPanel
	{
	private:
		std::shared_ptr<Scene> m_Scene;	//�����ʾ�ĳ���
		Object m_SelectionObject;		//��ѡ����ʵ��

		/// <summary>
		/// ����������
		/// </summary>
		/// <param name="Object">����</param>
		void DrawObjectNode(Object Object);
	public:
		SceneHierarchyPanel() = default;

		/// <summary>
		/// ��������㼶���
		/// </summary>
		/// <param name="context">����</param>
		SceneHierarchyPanel(const std::shared_ptr<Scene>& scene);

		/// <summary>
		/// ���ó�������
		/// </summary>
		/// <param name="context">����</param>
		void SetScene(const std::shared_ptr<Scene>& scene);

		/// <summary>
		/// ��ȾImGui
		/// </summary>
		void OnImGuiRender();
	};
}