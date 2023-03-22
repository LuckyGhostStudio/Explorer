#include "EditorLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Explorer/Scene/SceneSerializer.h"
#include "Explorer/Utils/PlatformUtils.h"

#include "ImGuizmo.h"
#include "Explorer/Math/Math.h"

namespace Explorer
{
	EditorLayer::EditorLayer() :Layer("EditorLayer")
	{
		
	}

	void EditorLayer::OnAttach()
	{
		FramebufferSpecification fbSpec;	//֡�������淶
		fbSpec.Attachments = {
			FramebufferTextureFormat::RGBA8,		//��ɫ������0��ʽ
			FramebufferTextureFormat::RED_INTEGER,	//��ɫ������1��ʽ����Ϊidʵ�������ʰȡ
			FramebufferTextureFormat::Depth			//��Ȼ�������ʽ
		};
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = std::make_shared<Framebuffer>(fbSpec);	//����֡������

		m_ActiveScene = std::make_shared<Scene>();								//��������
		m_EditorCamera = EditorCamera(30.0f, 1280.0f / 720.0f, 0.01f, 1000.0f);	//�����༭�����

		m_Camera = m_ActiveScene->CreateCameraObject();					//����Ĭ��Camera����
		m_Light = m_ActiveScene->CreateLightObject(Light::Type::Point);	//����Ĭ��Light����
		m_Cube = m_ActiveScene->CreateEmptyObject("Cube");				//����Ĭ��Cube����
		m_Cube.AddComponent<SpriteRenderer>();							//���SpriteRenderer��� TODO:���ڸ���ΪMesh���

#if 0

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
		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//����֡��������С

			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);							//�����ӿڴ�С
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//�����ӿڴ�С
		}

		if (m_ViewportFocused) {			//�ӿڱ��۽�
		}
			m_EditorCamera.OnUpdate(dt);	//���±༭�����

		//Renderer
		Renderer3D::ResetStats();	//����ͳ������
		m_Framebuffer->Bind();										//��֡������
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });	//����������ɫ
		RenderCommand::Clear();										//���

		m_Framebuffer->ClearAttachment(1, -1);	//�����ɫ������1������id��������Ϊ -1

		m_ActiveScene->OnUpdateEditor(dt, m_EditorCamera);	//���±༭������

		auto [mx, my] = ImGui::GetMousePos();	//���λ��
		//�������������ӿ����Ͻǵ�λ��
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;

		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];	//�ӿڴ�С������tab bar
		my = viewportSize.y - my;	//��תy���� ���½�0,0�������������½�Ϊ0,0��
		//���������ӿ����Ͻǵ�λ��
		int mouseX = (int)mx;
		int mouseY = (int)my;
		//������ӿ���
		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);	//��ȡ1����ɫ����������
			//�����ʰȡ������
			m_PickedObject = pixelData == -1 ? Object() : Object((entt::entity)pixelData, m_ActiveScene.get());
			//EXP_CORE_WARN("pixelData:{0}", pixelData);
		}

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
		ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
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
				//�����³���
				if (ImGui::MenuItem("New", "Ctrl+N")) {
					NewScene();
				}

				//���ļ������س���
				if (ImGui::MenuItem("Open...", "Ctrl+O")) {
					OpenScene();
				}

				//���Ϊ�����泡��
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
					SaveSceneAs();
				}

				//�˳�
				if (ImGui::MenuItem("Quit")) {
					Application::GetInstance().Close();	//�˳�����
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_HierarchyPanel.OnImGuiRender();	//��ȾHierarchy���

		//����Ⱦ����ͳ��
		ImGui::Begin("Renderer Stats");

		std::string name = m_PickedObject ? m_PickedObject.GetComponent<Name>().m_Name : "None";
		ImGui::Text("Hovered Object: %s", name.c_str());

		auto stats = Renderer3D::GetStats();
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Triangle Count: %d", stats.TriangleCount);
		ImGui::Text("Vertex Count: %d", stats.VertexCount);
		ImGui::Text("Index Count: %d", stats.IndexCount);
		ImGui::Text("FPS: %.3f", Application::GetInstance().GetFPS());	//֡��

		ImGui::End();	//Renderer Stats

		//�����ӿ�
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));	//����Gui������ʽ���߽�=0
		ImGui::Begin("Scene");
		
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();	//�ӿڿ���������Сֵ���ӿ����Ͻ�������ӿ����Ͻ�λ�ã�
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();	//�ӿڿ����������ֵ���ӿ����½�������ӿ����Ͻ�λ�ã�
		auto viewportOffset = ImGui::GetWindowPos();	//�ӿ�ƫ�������ӿ�������Ͻ�λ�ã��������Ļ���Ͻǣ�

		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();	//��ǰ���ڱ��۽�
		m_ViewportHovered = ImGui::IsWindowHovered();	//�����ͣ�ڵ�ǰ����

		Application::GetInstance().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);	//��ֹImGui�¼�

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();			//Scene�����������С
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };		//�ӿڴ�С

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();	//��ɫ������0 ID
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));	//�����ӿ�Image

		//Gizmo
		ImGuizmo::SetOrthographic(false);						//͸��ͶӰ
		ImGuizmo::SetDrawlist();								//���û����б�
		float windowWidth = (float)ImGui::GetWindowWidth();		//�ӿڿ�
		float windowHeight = (float)ImGui::GetWindowHeight();	//�ӿڸ�
		//���û�������
		ImGuizmo::SetRect(m_ViewportBounds[0].x, m_ViewportBounds[0].y, m_ViewportBounds[1].x - m_ViewportBounds[0].x, m_ViewportBounds[1].y - m_ViewportBounds[0].y);

		//�༭�����
		const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();	//ͶӰ����
		glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();				//��ͼ����

		//��������ϵ
		//ImGuizmo::DrawGrid(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), glm::value_ptr(glm::mat4(1.0f)), 10);

		Object selectedObject = m_HierarchyPanel.GetSelectedObject();	//��ѡ������
		//ѡ��������� && Gizmo���ʹ���
		if (selectedObject && m_GizmoType != -1) {
			////Camera
			//auto cameraObject = m_ActiveScene->GetPrimaryCameraObject();	//�����ʵ��
			//const auto& camera = cameraObject.GetComponent<Camera>();		//���
			//const glm::mat4& cameraProjection = camera.GetProjection();		//ͶӰ����
			//glm::mat4 cameraView = glm::inverse(cameraObject.GetComponent<Transform>().GetTransform());	//��ͼ����

			//��ѡ������transform
			auto& transformComponent = selectedObject.GetComponent<Transform>();	//Transform���
			glm::mat4 transform = transformComponent.GetTransform();

			bool span = Input::IsKeyPressed(Key::LeftControl);	//Ctrl�̶Ȳ�׽������ʱ�̶�delta�̶�
			float spanValue = 0.5f;	//ƽ�����ż����0.5m
			//��ת���ֵ��5��
			if (m_GizmoType == ImGuizmo::OPERATION::ROTATE) {
				spanValue = 5.0f;
			}

			float spanValues[3] = { spanValue, spanValue, spanValue };	//xyz��̶Ȳ�׽ֵ

			//���Ʋ���Gizmo�������ͼ���� ���ͶӰ���� Gizmo���� ��������ϵ ѡ������transform �������� �̶Ȳ�׽ֵ
			ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
				(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
				nullptr, span ? spanValues : nullptr);

			//Gizmo��ʹ��
			if (ImGuizmo::IsUsing()) {
				glm::vec3 position, rotation, scale;
				Math::DecomposeTransform(transform, position, rotation, scale);	//�ֽ�transform����

				glm::vec3 deltaRotation = rotation - transformComponent.m_Rotation;	//��ת����

				transformComponent.m_Position = position;		//����λ��
				transformComponent.m_Rotation += deltaRotation;	//������ת���ۼ������������������
				transformComponent.m_Scale = scale;				//��������
			}
		}
		
		ImGui::End();	//Scene
		ImGui::PopStyleVar();
		
		ImGui::End();	//DockSpace
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_EditorCamera.OnEvent(event);	//�༭������¼�

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(EXP_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));	//���ð��������¼�
		dispatcher.Dispatch<MouseButtonPressedEvent>(EXP_BIND_EVENT_FUNC(EditorLayer::OnMouseButtonPressed));	//������갴ť�����¼�
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		//��ݼ����ظ�����==0
		if (e.GetRepeatCount() > 0) {
			return false;
		}

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);	//Ctrl������
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);		//Shift������

		//�˵���ݼ�
		switch (e.GetKeyCode()) {
		case Key::N:
			if (control) {
				NewScene();		//�����³�����Ctrl+N
			}
			break;
		case Key::O:
			if (control) {
				OpenScene();	//�򿪳�����Ctrl+O
			}
			break;
		case Key::S:
			if (control && shift) {
				SaveSceneAs();	//�������Ϊ��Ctrl+Shift+S
			}
			break;
		}

		//Gizmo
		switch (e.GetKeyCode())
		{
			case Key::G:
				m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;	//ƽ��
				break;
			case Key::R:
				m_GizmoType = ImGuizmo::OPERATION::ROTATE;		//��ת
				break;
			case Key::S:
				m_GizmoType = ImGuizmo::OPERATION::SCALE;		//����
				break;
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		//����������
		if (e.GetMouseButton() == Mouse::ButtonLeft) {
			//������ӿ��� Gizmo����û����
			if (m_ViewportHovered && !ImGuizmo::IsOver()) {
				m_HierarchyPanel.SetSelectedObject(m_PickedObject);	//���ñ�ѡ������
			}
		}

		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = std::make_shared<Scene>();	//�����³���
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//�����ӿڴ�С
		m_HierarchyPanel.SetScene(m_ActiveScene);	//����Hierarchy�ĳ���
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Explorer Scene(*.explor)\0*.explor\0");	//���ļ��Ի����ļ�������\0 �ļ�����.explor��
		//·����Ϊ��
		if (!filepath.empty()) {
			m_ActiveScene = std::make_shared<Scene>();	//�����³���
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//�����ӿڴ�С
			m_HierarchyPanel.SetScene(m_ActiveScene);	//����Hierarchy�ĳ���

			SceneSerializer serializer(m_ActiveScene);	//�������л���
			serializer.Deserialize(filepath);			//�����л��������ļ��������³���
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Explorer Scene(*.explor)\0*.explor\0");	//�����ļ��Ի����ļ�������\0 �ļ�����.explor��
		//·����Ϊ��
		if (!filepath.empty()) {
			SceneSerializer serializer(m_ActiveScene);	//�������л���
			serializer.Serialize(filepath);				//���л������泡��
		}
	}
}