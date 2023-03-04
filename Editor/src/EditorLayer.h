#pragma once

#include "Explorer.h"

namespace Explorer
{
	/// <summary>
	/// 2D��Ⱦ���Բ�
	/// </summary>
	class EditorLayer :public Layer
	{
	private:
		//Temp
		std::shared_ptr<VertexArray> m_SquareVA;			//�����ζ�������
		std::shared_ptr<Texture2D> m_CheckerboardTexture;	//��������
		std::shared_ptr<Framebuffer> m_Framebuffer;			//֡������

		std::shared_ptr<Scene> m_ActiveScene;	//�����
		Object m_SquareObject;					//�����ζ���
		Object m_CameraObject;					//�������

		bool m_ViewportFocused = false;				//�ӿڱ��۽�
		bool m_ViewportHovered = false;				//�����ͣ���ӿ�
		glm::vec2 m_ViewportSize = { 0.0f, 0.0f };	//�ӿڴ�С

		glm::vec3 m_SquarePosition = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_SquareRotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_SquareScale = { 1.0f, 1.0f, 1.0f };

		glm::vec2 m_TextureTilingFactor = { 1.0f, 1.0f };	//�����ظ�����

		glm::vec4 m_SquareColor = { 1.0f, 1.0f, 1.0f, 1.0f };

		float fps = 0.0f;

		//���ܲ��Խ��
		struct ProfileResult
		{
			const char* Name;	//���Գ������
			float Time;			//����ʱ��
		};

		std::vector<ProfileResult> m_ProfileResults;	//���ܲ��Խ������
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
	};
}