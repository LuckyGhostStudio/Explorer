#include "EditorLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Explorer/Scene/SceneSerializer.h"

namespace Explorer
{
	EditorLayer::EditorLayer() :Layer("EditorLayer")//, m_CameraController(1280.0f / 720.0f) 
	{
		
	}

	void EditorLayer::OnAttach()
	{
		FramebufferSpecification fbSpec;	//֡�������淶
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = std::make_shared<Framebuffer>(fbSpec);	//����֡������

		m_ActiveScene = std::make_shared<Scene>();	//��������
#if 0
		Object cube = m_ActiveScene->CreateObject("Cube");				//����Cube����
		cube.AddComponent<SpriteRenderer>(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));	//���SpriteRenderer���

		m_CameraObject = m_ActiveScene->CreateObject("Main Camera");	//�����������
		m_CameraObject.AddComponent<Camera>();							//���Camera���

		class CameraController :public ScriptableObject
		{
		public:
			void OnCreate()
			{
				std::cout << "OnCreate" << std::endl;
			}

			void OnUpdate(DeltaTime dt)
			{
				auto& transform = GetComponent<Transform>();
				float speed = 5.0f;

				if (Input::IsKeyPressed(KeyCode::W)) {		//��
					transform.m_Position.y += speed * dt;
				}
				if (Input::IsKeyPressed(KeyCode::S)) {		//��
					transform.m_Position.y -= speed * dt;
				}
				if (Input::IsKeyPressed(KeyCode::A)) {		//��
					transform.m_Position.x -= speed * dt;
				}
				if (Input::IsKeyPressed(KeyCode::D)) {		//��
					transform.m_Position.x += speed * dt;
				}
			}

			void OnDestroy()
			{

			}
		};

		m_CameraObject.AddComponent<NativeScript>().Bind<CameraController>();	//��ӽű���� �� ��CameraController�ű�
#endif
		m_HierarchyPanel.SetScene(m_ActiveScene);	//����Hierarchy���ĳ���
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(DeltaTime dt)
	{
		fps = 1.0f / dt;	//֡��

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//����֡��������С
			//m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);				//���������С

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//�����ӿڴ�С
		}

		if (m_ViewportFocused) {	//�ӿڱ��۽�
			//m_CameraController.OnUpdate(dt);	//�������������
		}

		//Renderer
		Renderer2D::ResetStats();	//����ͳ������
		m_Framebuffer->Bind();										//��֡������
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });	//����������ɫ
		RenderCommand::Clear();										//���

		m_ActiveScene->OnUpdate(dt);	//���³���

		m_Framebuffer->Unbind();	//�����֡������
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

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

		// When using ImGuiDockNodeFlags_PassthruCentralNode, DockSpace() will render our background and handle the pass-thru hole, so we ask Begin() to not render a background.
		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		// Important: note that we proceed even if Begin() returns false (aka window is collapsed).
		// This is because we want to keep our DockSpace() active. If a DockSpace() is inactive, 
		// all active windows docked into it will lose their parent and become undocked.
		// We cannot preserve the docking relationship between an active window and an inactive docking, otherwise 
		// any change of dockspace/settings would lead to windows being stuck in limbo and never being visible.
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace Demo", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// ͣ���ռ�
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();	//��ʽ
		float minWinSizeX = style.WindowMinSize.x;	//��С���ڴ�С
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		//�˵���
		if (ImGui::BeginMenuBar())
		{
			//�˵���File
			if (ImGui::BeginMenu("File"))
			{
				if (ImGui::MenuItem("Serialize")) {		//�˵�����л������泡����
					SceneSerializer serializer(m_ActiveScene);				//m_ActiveScene�������л���
					serializer.Serialize("asserts/scenes/Example.explor");	//���л�
				}
				if (ImGui::MenuItem("Deserialize")) {	//�˵�������л������س�����
					SceneSerializer serializer(m_ActiveScene);				//m_ActiveScene�������л���
					serializer.Deserialize("asserts/scenes/Example.explor");	//�����л�
				}
				if (ImGui::MenuItem("Exit")) {	//�˵���˳�
					Application::GetInstance().Close();	//�˳�����
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_HierarchyPanel.OnImGuiRender();	//��ȾHierarchy���

		//����Ⱦ����ͳ��
		ImGui::Begin("Stats");
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

		m_ViewportFocused = ImGui::IsWindowFocused();	//��ǰ���ڱ��۽�
		m_ViewportHovered = ImGui::IsWindowHovered();	//�����ͣ�ڵ�ǰ����

		Application::GetInstance().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);	//��ֹImGui�¼�

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();			//Gui����С

		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };		//�ӿڴ�С

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();	//��ɫ������ID
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
		ImGui::End();
		ImGui::PopStyleVar();
		
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		//m_CameraController.OnEvent(event);	//��������¼�����
	}
}