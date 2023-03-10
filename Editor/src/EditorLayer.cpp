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
		FramebufferSpecification fbSpec;	//帧缓冲区规范
		fbSpec.Attachments = { FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::RGBA8, FramebufferTextureFormat::Depth };
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = std::make_shared<Framebuffer>(fbSpec);	//创建帧缓冲区

		m_ActiveScene = std::make_shared<Scene>();						//创建场景
		m_EditorCamera = EditorCamera(30.0f, 1280.0f / 720.0f, 0.01f, 1000.0f);	//创建编辑器相机

		m_Camera = m_ActiveScene->CreateObject("Camera");	//创建默认相机对象
		m_Camera.AddComponent<Camera>();					//添加Camera组件
		m_Cube = m_ActiveScene->CreateObject("Cube");		//创建默认Cube
		m_Cube.AddComponent<SpriteRenderer>(glm::vec4(0.0f, 0.0f, 1.0f, 1.0f));	//添加SpriteRenderer组件
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

				if (Input::IsKeyPressed(KeyCode::W)) {		//上
					transform.m_Position.y += speed * dt;
				}
				if (Input::IsKeyPressed(KeyCode::S)) {		//下
					transform.m_Position.y -= speed * dt;
				}
				if (Input::IsKeyPressed(KeyCode::A)) {		//左
					transform.m_Position.x -= speed * dt;
				}
				if (Input::IsKeyPressed(KeyCode::D)) {		//右
					transform.m_Position.x += speed * dt;
				}
			}

			void OnDestroy()
			{

			}
		};

		m_CameraObject.AddComponent<NativeScript>().Bind<CameraController>();	//添加脚本组件 并 绑定CameraController脚本
#endif
		m_HierarchyPanel.SetScene(m_ActiveScene);	//设置Hierarchy面板的场景
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(DeltaTime dt)
	{
		fps = 1.0f / dt;	//帧率

		if (FramebufferSpecification spec = m_Framebuffer->GetSpecification();
			m_ViewportSize.x > 0.0f && m_ViewportSize.y > 0.0f &&
			(spec.Width != m_ViewportSize.x || spec.Height != m_ViewportSize.y))
		{
			m_Framebuffer->Resize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//重置帧缓冲区大小

			m_EditorCamera.SetViewportSize(m_ViewportSize.x, m_ViewportSize.y);							//设置视口大小
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//重置视口大小
		}

		if (m_ViewportFocused) {			//视口被聚焦
		}
			m_EditorCamera.OnUpdate(dt);	//更新编辑器相机

		//Renderer
		Renderer2D::ResetStats();	//重置统计数据
		m_Framebuffer->Bind();										//绑定帧缓冲区
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });	//设置清屏颜色
		RenderCommand::Clear();										//清除

		m_ActiveScene->OnUpdateEditor(dt, m_EditorCamera);	//更新编辑器场景

		m_Framebuffer->Unbind();	//解除绑定帧缓冲区
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

		// 停靠空间
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();	//样式
		float minWinSizeX = style.WindowMinSize.x;	//最小窗口大小
		style.WindowMinSize.x = 370.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}

		style.WindowMinSize.x = minWinSizeX;

		//菜单条
		if (ImGui::BeginMenuBar())
		{
			//菜单：File
			if (ImGui::BeginMenu("File"))
			{
				//创建新场景
				if (ImGui::MenuItem("New", "Ctrl+N")) {
					NewScene();
				}

				//打开文件：加载场景
				if (ImGui::MenuItem("Open...", "Ctrl+O")) {
					OpenScene();
				}

				//另存为：保存场景
				if (ImGui::MenuItem("Save As...", "Ctrl+Shift+S")) {
					SaveSceneAs();
				}

				//退出
				if (ImGui::MenuItem("Quit")) {
					Application::GetInstance().Close();	//退出程序
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_HierarchyPanel.OnImGuiRender();	//渲染Hierarchy面板

		//批渲染数据统计
		ImGui::Begin("Stats");
		auto stats = Renderer2D::GetStats();
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::Text("FPS: %.3f", fps);	//帧率
		ImGui::End();

		//场景视口
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));	//设置Gui窗口样式：边界=0
		ImGui::Begin("Scene");
		{
			m_ViewportFocused = ImGui::IsWindowFocused();	//当前窗口被聚焦
			m_ViewportHovered = ImGui::IsWindowHovered();	//鼠标悬停在当前窗口

			Application::GetInstance().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);	//阻止ImGui事件

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();			//Gui面板大小
			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };		//视口大小

			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();	//颜色缓冲区0 ID
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));	//场景视口Image

			//Gizmo
			ImGuizmo::SetOrthographic(false);						//透视投影
			ImGuizmo::SetDrawlist();								//设置绘制列表
			float windowWidth = (float)ImGui::GetWindowWidth();		//视口宽
			float windowHeight = (float)ImGui::GetWindowHeight();	//视口高
			ImGuizmo::SetRect(ImGui::GetWindowPos().x, ImGui::GetWindowPos().y, windowWidth, windowHeight);	//设置绘制区域

			//编辑器相机
			const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();	//投影矩阵
			glm::mat4 cameraView = m_EditorCamera.GetViewMatrix();				//视图矩阵

			//绘制坐标系
			//ImGuizmo::DrawGrid(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection), glm::value_ptr(glm::mat4(1.0f)), 10);

			Object selectedObject = m_HierarchyPanel.GetSelectedObject();	//被选中物体
			//选中物体存在 && Gizmo类型存在
			if (selectedObject && m_GizmoType != -1) {
				////Camera
				//auto cameraObject = m_ActiveScene->GetPrimaryCameraObject();	//主相机实体
				//const auto& camera = cameraObject.GetComponent<Camera>();		//相机
				//const glm::mat4& cameraProjection = camera.GetProjection();		//投影矩阵
				//glm::mat4 cameraView = glm::inverse(cameraObject.GetComponent<Transform>().GetTransform());	//视图矩阵

				//被选中物体transform
				auto& transformComponent = selectedObject.GetComponent<Transform>();	//Transform组件
				glm::mat4 transform = transformComponent.GetTransform();

				bool span = Input::IsKeyPressed(Key::LeftControl);	//Ctrl刻度捕捉：操作时固定delta刻度
				float spanValue = 0.5f;	//平移缩放间隔：0.5m
				//旋转间隔值：5度
				if (m_GizmoType == ImGuizmo::OPERATION::ROTATE) {
					spanValue = 5.0f;
				}

				float spanValues[3] = { spanValue, spanValue, spanValue };	//xyz轴刻度捕捉值

				//绘制操作Gizmo：相机视图矩阵 相机投影矩阵 Gizmo类型 本地坐标系 选中物体transform 增量矩阵 刻度捕捉值
				ImGuizmo::Manipulate(glm::value_ptr(cameraView), glm::value_ptr(cameraProjection),
					(ImGuizmo::OPERATION)m_GizmoType, ImGuizmo::LOCAL, glm::value_ptr(transform),
					nullptr, span ? spanValues : nullptr);

				//Gizmo被使用
				if (ImGuizmo::IsUsing()) {
					glm::vec3 position, rotation, scale;
					Math::DecomposeTransform(transform, position, rotation, scale);	//分解transform矩阵

					glm::vec3 deltaRotation = rotation - transformComponent.m_Rotation;	//旋转增量

					transformComponent.m_Position = position;		//更新位置
					transformComponent.m_Rotation += deltaRotation;	//更新旋转：累加增量，避免万向节锁
					transformComponent.m_Scale = scale;				//更新缩放
				}
			}
		}
		ImGui::End();
		ImGui::PopStyleVar();
		
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_EditorCamera.OnEvent(event);	//编辑器相机事件

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(EXP_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));	//调用按键按下事件
	}

	bool EditorLayer::OnKeyPressed(KeyPressedEvent& e)
	{
		//快捷键：重复次数==0
		if (e.GetRepeatCount() > 0) {
			return false;
		}

		bool control = Input::IsKeyPressed(Key::LeftControl) || Input::IsKeyPressed(Key::RightControl);	//Ctrl键按下
		bool shift = Input::IsKeyPressed(Key::LeftShift) || Input::IsKeyPressed(Key::RightShift);		//Shift键按下

		//菜单快捷键
		switch (e.GetKeyCode()) {
		case Key::N:
			if (control) {
				NewScene();		//创建新场景：Ctrl+N
			}
			break;
		case Key::O:
			if (control) {
				OpenScene();	//打开场景：Ctrl+O
			}
			break;
		case Key::S:
			if (control && shift) {
				SaveSceneAs();	//场景另存为：Ctrl+Shift+S
			}
			break;
		}

		//Gizmo
		switch (e.GetKeyCode())
		{
		case Key::G:
			m_GizmoType = ImGuizmo::OPERATION::TRANSLATE;	//平移
			break;
		case Key::R:
			m_GizmoType = ImGuizmo::OPERATION::ROTATE;		//旋转
			break;
		case Key::S:
			m_GizmoType = ImGuizmo::OPERATION::SCALE;		//缩放
			break;
		}
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = std::make_shared<Scene>();	//创建新场景
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//重置视口大小
		m_HierarchyPanel.SetScene(m_ActiveScene);	//设置Hierarchy的场景
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Explorer Scene(*.explor)\0*.explor\0");	//打开文件对话框（文件类型名\0 文件类型.explor）
		//路径不为空
		if (!filepath.empty()) {
			m_ActiveScene = std::make_shared<Scene>();	//创建新场景
			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//重置视口大小
			m_HierarchyPanel.SetScene(m_ActiveScene);	//设置Hierarchy的场景

			SceneSerializer serializer(m_ActiveScene);	//场景序列化器
			serializer.Deserialize(filepath);			//反序列化：加载文件场景到新场景
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Explorer Scene(*.explor)\0*.explor\0");	//保存文件对话框（文件类型名\0 文件类型.explor）
		//路径不为空
		if (!filepath.empty()) {
			SceneSerializer serializer(m_ActiveScene);	//场景序列化器
			serializer.Serialize(filepath);				//序列化：保存场景
		}
	}
}