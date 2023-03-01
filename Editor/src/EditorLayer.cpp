#include "EditorLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Explorer
{
	EditorLayer::EditorLayer() :Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f) {}

	void EditorLayer::OnAttach()
	{
		m_CheckerboardTexture = std::make_shared<Texture2D>("asserts/textures/Checkerboard.png");	//��������

		FramebufferSpecification fbSpec;	//֡�������淶
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = std::make_shared < Framebuffer>(fbSpec);	//����֡������
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(DeltaTime dt)
	{
		fps = 1.0f / dt;	//֡��

		m_CameraController.OnUpdate(dt);	//�������������

		//Renderer
		Renderer2D::ResetStats();	//����ͳ������
		m_Framebuffer->Bind();										//��֡������
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });	//����������ɫ
		RenderCommand::Clear();										//���

		Renderer2D::BeginScene(m_CameraController.GetCamera());		//��ʼ��Ⱦ����

		Renderer2D::DrawQuad(m_SquarePosition, m_SquareRotation.z, m_SquareScale, m_SquareColor, m_CheckerboardTexture, m_TextureTilingFactor);	//�����ı���
		Renderer2D::DrawQuad({ -3.0f, 1.0f, 1.0f }, 0.0f, { 0.8f, 0.8f, 1.0f }, { 0.8f, 0.2f, 0.5f, 1.0f }, m_CheckerboardTexture, { 0.5f, 0.5f });	//�����ı���
		Renderer2D::DrawQuad({ 1.0f, 0.0f, 1.0f }, 0.0f, { 0.8f, 0.8f, 1.0f }, { 0.8f, 0.2f, 0.3f, 0.8f });	//�����ı���
		Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.0f }, 0.0f, { 0.5f, 0.75f, 0.0f }, { 0.2f, 0.8f, 0.3f, 1.0f });	//�����ı���

		Renderer2D::EndScene();						//������Ⱦ����

		Renderer2D::BeginScene(m_CameraController.GetCamera());		//��ʼ��Ⱦ����

		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.6f };
				Renderer2D::DrawQuad({ x, y }, 0.0f, { 0.45f, 0.45f }, color);
			}
		}

		Renderer2D::EndScene();						//������Ⱦ����
		m_Framebuffer->Unbind();	//�����֡������
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool dockSpaceOpen = true;	//ͣ���ռ��Ƿ���
		static bool opt_fullscreen_persistant = true;
		static ImGuiDockNodeFlags opt_flags = ImGuiDockNodeFlags_None;
		bool opt_fullscreen = opt_fullscreen_persistant;

		// We are using the ImGuiWindowFlags_NoDocking flag to make the parent window not dockable into,
		// because it would be confusing to have two docking targets within each others.
		ImGuiWindowFlags window_flags = ImGuiWindowFlags_MenuBar | ImGuiWindowFlags_NoDocking;
		if (opt_fullscreen)
		{
			ImGuiViewport* viewport = ImGui::GetMainViewport();
			ImGui::SetNextWindowPos(viewport->Pos);
			ImGui::SetNextWindowSize(viewport->Size);
			ImGui::SetNextWindowViewport(viewport->ID);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowRounding, 0.0f);
			ImGui::PushStyleVar(ImGuiStyleVar_WindowBorderSize, 0.0f);
			window_flags |= ImGuiWindowFlags_NoTitleBar | ImGuiWindowFlags_NoCollapse | ImGuiWindowFlags_NoResize | ImGuiWindowFlags_NoMove;
			window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus | ImGuiWindowFlags_NoNavFocus;
		}

		// When using ImGuiDockNodeFlags_PassthruDockspace, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (opt_flags & ImGuiDockNodeFlags_PassthruDockspace)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));

		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);	//ͣ���ռ�
		{
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			//UIͣ���ռ�
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
			}

			//�˵���
			if (ImGui::BeginMenuBar())
			{
				//�˵���File
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Exit")) {	//�˵���˳�
						Application::GetInstance().Close();	//�˳�����
					}
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			//�������
			ImGui::Begin("Settings");
			ImGui::ColorEdit4("Color", glm::value_ptr(m_SquareColor));	//��ɫ�༭UI
			ImGui::SliderFloat3("Position", glm::value_ptr(m_SquarePosition), -10.0f, 10.0f);
			ImGui::SliderFloat3("Rotation", glm::value_ptr(m_SquareRotation), -360.0f, 360.0f);
			ImGui::SliderFloat3("Scale", glm::value_ptr(m_SquareScale), 0.0f, 10.0f);
			ImGui::SliderFloat2("Texture Tiling Factor", glm::value_ptr(m_TextureTilingFactor), 0.0f, 10.0f);
			ImGui::End();

			//����Ⱦ����ͳ��
			ImGui::Begin("Renderer2D Stats");
			auto stats = Renderer2D::GetStats();
			ImGui::Text("Draw Calls: %d", stats.DrawCalls);
			ImGui::Text("Quad: %d", stats.QuadCount);
			ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
			ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
			ImGui::Text("FPS: %.3f", fps);	//֡��
			ImGui::End();

			//�����ӿ�
			ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));	//����Gui������ʽ���߽�=0
			ImGui::Begin("Scene");
			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();			//Gui����С
			//�ӿڴ�С != Gui����С
			if (m_ViewportSize != (*(glm::vec2*)&viewportPanelSize)) {
				m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);	//����֡��������С
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };			//�ӿڴ�С
				m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);	//���������С
			}
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();	//��ɫ������ID
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
			ImGui::PopStyleVar();
		}
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_CameraController.OnEvent(event);	//��������¼�����
	}
}