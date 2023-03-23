#pragma once

#include "Explorer.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Explorer/Renderer/EditorCamera.h"

namespace Explorer
{
	/// <summary>
	/// 2D��Ⱦ���Բ�
	/// </summary>
	class EditorLayer :public Layer
	{
	private:
		std::shared_ptr<Framebuffer> m_Framebuffer;			//֡������

		std::shared_ptr<Scene> m_ActiveScene;	//�����
		EditorCamera m_EditorCamera;			//�༭�����

		Object m_Camera;						//����Ĭ�����
		Object m_Cube;							//����Ĭ��������
		Object m_Light;							//����Ĭ�Ϲ�Դ

		Object m_PickedObject;					//�����ʰȡ������

		bool m_ViewportFocused = false;				//�ӿڱ��۽�
		bool m_ViewportHovered = false;				//�����ͣ���ӿ�

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };	//�ӿڴ�С
		glm::vec2 m_ViewportBounds[2];				//�ӿڱ߽�λ��

		int m_GizmoType = -1;	//Gizmo�������� -1�� 0ƽ�� 1��ת 2����

		SceneHierarchyPanel m_HierarchyPanel;	//����Hierarchy���s
	public:
		EditorLayer();
		virtual ~EditorLayer() = default;

		/// <summary>
		/// ���ò���ӵ���ջʱ����
		/// </summary>
		virtual void OnAttach() override;

		/// <summary>
		/// ���ò�Ӳ�ջ�Ƴ�ʱ����
		/// </summary>
		virtual void OnDetach() override;

		/// <summary>
		/// �ò����ʱ���ã�ÿ֡����
		/// </summary>
		/// <param name="dt">֡���</param>
		virtual void OnUpdate(DeltaTime dt) override;

		/// <summary>
		/// ��ȾImGui
		/// </summary>
		virtual void OnImGuiRender() override;

		/// <summary>
		/// �¼������������¼�
		/// </summary>
		/// <param name="event">�¼�</param>
		virtual void OnEvent(Event& event) override;

		/// <summary>
		/// �����³���
		/// </summary>
		void NewScene();

		/// <summary>
		/// �򿪳���
		/// </summary>
		void OpenScene();

		/// <summary>
		/// �������Ϊ
		/// </summary>
		void SaveSceneAs();
	private:
		/// <summary>
		/// ��������ʱ����
		/// </summary>
		/// <param name="e">���������¼�</param>
		/// <returns>������</returns>
		bool OnKeyPressed(KeyPressedEvent& e);

		/// <summary>
		/// ��갴ť����ʱ����
		/// </summary>
		/// <param name="e">��갴ť�����¼�</param>
		/// <returns>������</returns>
		bool OnMouseButtonPressed(MouseButtonPressedEvent& e);
	};
}