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
	extern const std::filesystem::path g_AssetPath;	//�ʲ�Ŀ¼��ȫ�֣�

	EditorLayer::EditorLayer() :Layer("EditorLayer")
	{
		
	}

	void EditorLayer::OnAttach()
	{
		m_PlayIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/ToolBar/PlayButton.png");	//Play��ťͼ��

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

		m_MainCamera = m_ActiveScene->CreateCameraObject("Main Camera", true);	//����Ĭ��Camera���������
		m_Light = m_ActiveScene->CreateLightObject();							//����Ĭ��Light����Point��
		m_Cube = m_ActiveScene->CreateMeshObject("Cube", Mesh::Type::Cube);		//����Ĭ��Cube����

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
		m_SceneHierarchyPanel.SetScene(m_ActiveScene);	//����Hierarchy���ĳ���
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

		//Renderer
		Renderer3D::ResetStats();	//����ͳ������

		m_Framebuffer->Bind();										//��֡������

		glm::vec4& defaultClearColor = m_ActiveScene->GetEnvironment().GetNoneSkyboxColor();	//Ĭ��������ɫ:��պ�ȱʧĬ����ɫ

		//����״̬
		switch (m_SceneState)
		{
			case SceneState::Edit:	//�༭״̬
				RenderCommand::SetClearColor(defaultClearColor);	//����������ɫ:��պ�ȱʧĬ����ɫ
				RenderCommand::Clear();								//���
				m_Framebuffer->ClearAttachment(1, -1);				//�����ɫ������1������id��������Ϊ -1

				m_EditorCamera.OnUpdate(dt);						//���±༭�����
				m_ActiveScene->OnUpdateEditor(dt, m_EditorCamera);	//���±༭������
				break;
			case SceneState::Play:	//����״̬
				Camera& mainCamera = m_ActiveScene->GetPrimaryCameraObject().GetComponent<Camera>();	//���������
				glm::vec4& clearColor = mainCamera.GetClearFlag() == Camera::ClearFlag::Color ? mainCamera.GetBackgroundColor() : defaultClearColor;	//������ɫ
				RenderCommand::SetClearColor(clearColor);			//����������ɫ:�������������ɫ
				RenderCommand::Clear();								//���
				m_Framebuffer->ClearAttachment(1, -1);				//�����ɫ������1������id��������Ϊ -1

				m_ActiveScene->OnUpdate(dt);						//����ʱ��������
				break;
		}

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
			EXP_CORE_WARN("pixelData:{0}", pixelData);
			EXP_CORE_WARN("mx:{0}, my:{1}", mouseX, mouseY);
		}

		m_Framebuffer->Unbind();	//�����֡������
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool dockspaceOpen = true;
		static bool opt_fullscreen_persistant = true;
		bool opt_fullscreen = opt_fullscreen_persistant;
		static ImGuiDockNodeFlags dockspace_flags = ImGuiDockNodeFlags_None;

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

		if (dockspace_flags & ImGuiDockNodeFlags_PassthruCentralNode)
			window_flags |= ImGuiWindowFlags_NoBackground;

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0.0f, 0.0f));
		ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
		ImGui::PopStyleVar();

		if (opt_fullscreen)
			ImGui::PopStyleVar(2);

		// ͣ���ռ�����
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();	//��ʽ
		float minWinSizeX = style.WindowMinSize.x;	//��С���ڴ�С
		style.WindowMinSize.x = 420.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeX;
		style.FrameRounding = 4.0f;				//�ؼ��߿�Բ��
		style.WindowRounding = 4.0f;			//���ڱ߿�Բ��
		style.GrabRounding = 4.0f;				//�϶���handleԲ��
		style.PopupRounding = 4.0f;				//��������Բ��

		style.FrameBorderSize = 1.0f;			//�߿�ߴ�
		style.WindowMenuButtonPosition = -1;	//����tabbar��ťȡ����ʾ

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

				ImGui::Separator();
				//����3D����
				if (ImGui::BeginMenu("Import")) {
					//.obj�ļ�
					if (ImGui::MenuItem("Wavefront(.obj)")) {
						//TODO:����.obj�ļ�
						// TODO:����SubMesh
						//TODO:����Mesh
					}
					ImGui::EndMenu();
				}

				//������������
				if (ImGui::BeginMenu("Export")) {
					//.obj�ļ�
					if (ImGui::MenuItem("Wavefront(.obj)")) {
						//TODO:����Ϊ.obj�ļ�
					}
					ImGui::EndMenu();
				}
				ImGui::Separator();

				//�˳�
				if (ImGui::MenuItem("Quit")) {
					Application::GetInstance().Close();	//�˳�����
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();	//��ȾHierarchy���
		m_ContentBrowserPanel.OnImGuiRender();	//��ȾProject���

		//����Ⱦ����ͳ��
		ImGui::Begin("Renderer Stats");

		//std::string name = m_PickedObject ? m_PickedObject.GetComponent<Self>().GetObjectName() : "None";
		//ImGui::Text("Hovered Object: %s", name.c_str());

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
		UI_ToolBar();	//������

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();	//�ӿڿ���������Сֵ���ӿ����Ͻ�������ӿ����Ͻ�λ�ã�
		viewportMinRegion.y += 34;	//TODO:����ƫ�ƹ������ĸ߶�
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();	//�ӿڿ����������ֵ���ӿ����½�������ӿ����Ͻ�λ�ã�
		auto viewportOffset = ImGui::GetWindowPos();	//�ӿ�ƫ�������ӿ�������Ͻ�λ�ã��������Ļ���Ͻǣ�

		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();	//��ǰ���ڱ��۽�
		m_ViewportHovered = ImGui::IsWindowHovered();	//�����ͣ�ڵ�ǰ����

		Application::GetInstance().GetImGuiLayer()->BlockEvents(/*!m_ViewportFocused && */!m_ViewportHovered);	//��ֹImGui�¼�

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();			//Scene�����������С
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };		//�ӿڴ�С

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();	//��ɫ������0 ID
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));	//�����ӿ�Image

		//������קԴ��Project��壩���Ƶ������Ϸŵ�Ŀ�꣨������	������·��
		ContentBrowserPanel::DragDropToTarget({ ".explor" }, [&](const std::filesystem::path& scenePath)
		{
			OpenScene(scenePath);	//�򿪳���
		});

		//�༭�����
		const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();	//ͶӰ����
		const glm::mat4& cameraView = m_EditorCamera.GetViewMatrix();				//��ͼ����
		
		Gizmo::Init(m_ViewportBounds[0], m_ViewportBounds[1]);												//��ʼ��Gizmo���Ʋ���	
		Gizmo::DrawTransformation(m_SceneHierarchyPanel.GetSelectedObject(), cameraView, cameraProjection);	//����Transform Gizmos
		
		ImGui::End();	//Scene
		ImGui::PopStyleVar();

		//ImGui::ShowDemoWindow();	//��������
		
		ImGui::End();	//DockSpace
	}

	void EditorLayer::UI_ToolBar()
	{
		float buttonSize = 20.0f;
		static ImVec4 playButtonColor = { 0.2f, 0.205f, 0.21f, 1.0f };	//Play��ť��ɫ

		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.5f - buttonSize * 0.5f);	//��ť����
		ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMin().y + 4.0f);
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, playButtonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { playButtonColor.x + 0.01f, playButtonColor.y + 0.01f, playButtonColor.z + 0.01f, 1.0f });
		//Play|Stop��ť ����
		if (ImGui::ImageButton((ImTextureID)m_PlayIcon->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1))) {
			//����״̬
			switch (m_SceneState)
			{
				case SceneState::Edit:	//�༭״̬
					playButtonColor = { 0.14f, 0.29f, 0.42f, 1.0f };	//��ɫ
					OnScenePlay();
					break;
				case SceneState::Play:	//����״̬
					playButtonColor = { 0.2f, 0.205f, 0.21f, 1.0f };
					OnSceneStop();
					break;
			}
		}
		ImGui::PopStyleVar();
		ImGui::PopStyleVar();
		ImGui::PopStyleColor();
		ImGui::PopStyleColor();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_EditorCamera.OnEvent(event);	//�༭������¼�

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(EXP_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));					//���ð��������¼�TODO:�����ڳ�������ʱ�Ŵ���
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
		switch (e.GetKeyCode()) 
		{
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

		//����Transform Gizmo����
		switch (e.GetKeyCode())
		{
			case Key::G:
				Gizmo::s_TransformType = (Gizmo::TransformationType)ImGuizmo::OPERATION::TRANSLATE;	//ƽ��
				break;
			case Key::R:
				Gizmo::s_TransformType = (Gizmo::TransformationType)ImGuizmo::OPERATION::ROTATE;	//��ת
				break;
			case Key::S:
				Gizmo::s_TransformType = (Gizmo::TransformationType)ImGuizmo::OPERATION::SCALE;		//����
				break;
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		//����������
		if (e.GetMouseButton() == Mouse::ButtonLeft) {
			//������ӿ��� Gizmo����û����
			if (m_ViewportHovered && !ImGuizmo::IsOver()) {
				m_SceneHierarchyPanel.SetSelectedObject(m_PickedObject);	//���ñ�ѡ������
			}
		}

		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = std::make_shared<Scene>("New Scene");			//�����³���

		m_MainCamera = m_ActiveScene->CreateCameraObject("Main Camera", true);	//����Ĭ��Camera���������
		m_Light = m_ActiveScene->CreateLightObject();							//����Ĭ��Light����
		m_Cube = m_ActiveScene->CreateMeshObject("Cube", Mesh::Type::Cube);		//����Ĭ��Cube����

		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//�����ӿڴ�С
		m_SceneHierarchyPanel.SetScene(m_ActiveScene);	//����Hierarchy�ĳ���
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Explorer Scene(*.explor)\0*.explor\0");	//���ļ��Ի����ļ�������\0 �ļ�����.explor��
		//·����Ϊ��
		if (!filepath.empty()) {
			OpenScene(filepath);	//�򿪳���
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		m_ActiveScene = std::make_shared<Scene>(path.stem().string());	//�����³�������������û����չ����
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//�����ӿڴ�С
		m_SceneHierarchyPanel.SetScene(m_ActiveScene);	//����Hierarchy�ĳ���

		SceneSerializer serializer(m_ActiveScene);	//�������л���
		serializer.Deserialize(path.string());		//�����л��������ļ��������³���
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

	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;	//��ʼ����
	}

	void EditorLayer::OnSceneStop()
	{
		m_SceneState = SceneState::Edit;	//ֹͣ����
	}
}