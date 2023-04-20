#pragma once

#include "Explorer.h"
#include "Panels/SceneHierarchyPanel.h"
#include "Panels/ContentBrowserPanel.h"
#include "Explorer/Renderer/EditorCamera.h"

namespace Explorer
{
	/// <summary>
	/// 2D��Ⱦ���Բ�
	/// </summary>
	class EditorLayer :public Layer
	{
	private:
		std::shared_ptr<Framebuffer> m_Framebuffer;	//֡������

		std::shared_ptr<Scene> m_ActiveScene;		//�����
		EditorCamera m_EditorCamera;				//�༭�����

		Object m_MainCamera;						//����Ĭ������������
		Object m_Cube;								//����Ĭ��������
		Object m_Light;								//����Ĭ�Ϲ�Դ

		Object m_PickedObject;						//�����ʰȡ������
		bool m_Pickable = true;						//�Ƿ��ʰȡ�����������ñ�ѡ������

		bool m_ViewportFocused = false;				//�ӿڱ��۽�
		bool m_ViewportHovered = false;				//�����ͣ���ӿ�

		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };	//�ӿڴ�С
		glm::vec2 m_ViewportBounds[2];				//�ӿڱ߽�λ��

		enum class SceneState
		{
			Edit = 0,	//�༭
			Play = 1	//����
		};

		SceneState m_SceneState = SceneState::Edit;	//����״̬
		
		SceneHierarchyPanel m_SceneHierarchyPanel;	//����Hierarchy���
		ContentBrowserPanel m_ContentBrowserPanel;	//��Ŀ�ļ�Ŀ¼Project������

		std::shared_ptr<Texture2D> m_SelectionIcon;		//ѡ������ ��ťͼ��

		std::shared_ptr<Texture2D> m_TranslationIcon;	//ƽ�� ��ťͼ��
		std::shared_ptr<Texture2D> m_RotationIcon;		//ѡ�� ��ťͼ��
		std::shared_ptr<Texture2D> m_ScaleIcon;			//���� ��ťͼ��

		std::shared_ptr<Texture2D> m_PlayIcon;	//Play��ťͼ��
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

		/// <summary>
		/// �����³���
		/// </summary>
		void NewScene();

		/// <summary>
		/// �򿪳���
		/// </summary>
		void OpenScene();

		/// <summary>
		/// �򿪳��������ļ�������򿪣�
		/// </summary>
		/// <param name="path">�����ļ�·��</param>
		void OpenScene(const std::filesystem::path& path);

		/// <summary>
		/// �������Ϊ
		/// </summary>
		void SaveSceneAs();

		/// <summary>
		/// ����ģ���ļ�
		/// </summary>
		void ImportModelFile();

		/// <summary>
		/// ����ģ���ļ�
		/// </summary>
		/// <param name="path">ģ���ļ�·��</param>
		void ImportModelFile(const std::filesystem::path& path);

		/// <summary>
		/// Play��ť����ʱ����
		/// </summary>
		void OnScenePlay();

		/// <summary>
		/// Stop��ť����ʱ����
		/// </summary>
		void OnSceneStop();

		/// <summary>
		/// ������
		/// </summary>
		void UI_ToolBar();
	};
}