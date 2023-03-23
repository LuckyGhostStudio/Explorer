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
		fbSpec.Attachments = {
			FramebufferTextureFormat::RGBA8,		//颜色缓冲区0格式
			FramebufferTextureFormat::RED_INTEGER,	//颜色缓冲区1格式：作为id实现鼠标点击拾取
			FramebufferTextureFormat::Depth			//深度缓冲区格式
		};
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = std::make_shared<Framebuffer>(fbSpec);	//创建帧缓冲区

		m_ActiveScene = std::make_shared<Scene>();								//创建场景
		m_EditorCamera = EditorCamera(30.0f, 1280.0f / 720.0f, 0.01f, 1000.0f);	//创建编辑器相机

		m_Camera = m_ActiveScene->CreateCameraObject();					//创建默认Camera对象
		m_Light = m_ActiveScene->CreateLightObject(Light::Type::Point);	//创建默认Light对象
		m_Cube = m_ActiveScene->CreateEmptyObject("Cube");				//创建默认Cube对象
		m_Cube.AddComponent<SpriteRenderer>();							//添加SpriteRenderer组件 TODO:后期更换为Mesh组件

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
		Renderer3D::ResetStats();	//重置统计数据
		m_Framebuffer->Bind();										//绑定帧缓冲区
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });	//设置清屏颜色
		RenderCommand::Clear();										//清除

		m_Framebuffer->ClearAttachment(1, -1);	//清除颜色缓冲区1（物体id缓冲区）为 -1

		m_ActiveScene->OnUpdateEditor(dt, m_EditorCamera);	//更新编辑器场景

		auto [mx, my] = ImGui::GetMousePos();	//鼠标位置
		//计算鼠标相对于视口左上角的位置
		mx -= m_ViewportBounds[0].x;
		my -= m_ViewportBounds[0].y;

		glm::vec2 viewportSize = m_ViewportBounds[1] - m_ViewportBounds[0];	//视口大小：包括tab bar
		my = viewportSize.y - my;	//翻转y坐标 左下角0,0（纹理坐标左下角为0,0）
		//鼠标相对于视口左上角的位置
		int mouseX = (int)mx;
		int mouseY = (int)my;
		//鼠标在视口内
		if (mouseX >= 0 && mouseY >= 0 && mouseX < (int)viewportSize.x && mouseY < (int)viewportSize.y) {
			int pixelData = m_Framebuffer->ReadPixel(1, mouseX, mouseY);	//读取1号颜色缓冲区像素
			//被鼠标拾取的物体
			m_PickedObject = pixelData == -1 ? Object() : Object((entt::entity)pixelData, m_ActiveScene.get());
			//EXP_CORE_WARN("pixelData:{0}", pixelData);
		}

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
		ImGui::Begin("DockSpace", &dockspaceOpen, window_flags);
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
		ImGui::Begin("Renderer Stats");

		std::string name = m_PickedObject ? m_PickedObject.GetComponent<Name>().m_Name : "None";
		ImGui::Text("Hovered Object: %s", name.c_str());

		auto stats = Renderer3D::GetStats();
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Triangle Count: %d", stats.TriangleCount);
		ImGui::Text("Vertex Count: %d", stats.VertexCount);
		ImGui::Text("Index Count: %d", stats.IndexCount);
		ImGui::Text("FPS: %.3f", Application::GetInstance().GetFPS());	//帧率

		ImGui::End();	//Renderer Stats

		//场景视口
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));	//设置Gui窗口样式：边界=0
		ImGui::Begin("Scene");
		
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();	//视口可用区域最小值（视口左上角相对于视口左上角位置）
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();	//视口可用区域最大值（视口右下角相对于视口左上角位置）
		auto viewportOffset = ImGui::GetWindowPos();	//视口偏移量：视口面板左上角位置（相对于屏幕左上角）

		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();	//当前窗口被聚焦
		m_ViewportHovered = ImGui::IsWindowHovered();	//鼠标悬停在当前窗口

		Application::GetInstance().GetImGuiLayer()->BlockEvents(!m_ViewportFocused && !m_ViewportHovered);	//阻止ImGui事件

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();			//Scene面板可用区域大小
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };		//视口大小

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();	//颜色缓冲区0 ID
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));	//场景视口Image

		//编辑器相机
		const glm::mat4& cameraProjection = m_EditorCamera.GetProjection();	//投影矩阵
		const glm::mat4& cameraView = m_EditorCamera.GetViewMatrix();		//视图矩阵

		
		Gizmo::Init(m_ViewportBounds[0], m_ViewportBounds[1]);											//初始化Gizmo绘制参数	
		Gizmo::DrawTransformation(m_HierarchyPanel.GetSelectedObject(), cameraView, cameraProjection);	//绘制Transform Gizmos
		
		ImGui::End();	//Scene
		ImGui::PopStyleVar();
		
		ImGui::End();	//DockSpace
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_EditorCamera.OnEvent(event);	//编辑器相机事件

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(EXP_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));	//调用按键按下事件
		dispatcher.Dispatch<MouseButtonPressedEvent>(EXP_BIND_EVENT_FUNC(EditorLayer::OnMouseButtonPressed));	//调用鼠标按钮按下事件
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

		//设置Transform Gizmo类型
		switch (e.GetKeyCode())
		{
			case Key::G:
				Gizmo::s_TransformType = (Gizmo::TransformationType)ImGuizmo::OPERATION::TRANSLATE;	//平移
				break;
			case Key::R:
				Gizmo::s_TransformType = (Gizmo::TransformationType)ImGuizmo::OPERATION::ROTATE;	//旋转
				break;
			case Key::S:
				Gizmo::s_TransformType = (Gizmo::TransformationType)ImGuizmo::OPERATION::SCALE;		//缩放
				break;
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		//鼠标左键按下
		if (e.GetMouseButton() == Mouse::ButtonLeft) {
			//鼠标在视口内 Gizmo控制没结束
			if (m_ViewportHovered && !ImGuizmo::IsOver()) {
				m_HierarchyPanel.SetSelectedObject(m_PickedObject);	//设置被选中物体
			}
		}

		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = std::make_shared<Scene>();	//创建新场景

		m_Camera = m_ActiveScene->CreateCameraObject();					//创建默认Camera对象
		m_Light = m_ActiveScene->CreateLightObject(Light::Type::Point);	//创建默认Light对象
		m_Cube = m_ActiveScene->CreateEmptyObject("Cube");				//创建默认Cube对象
		m_Cube.AddComponent<SpriteRenderer>();							//添加SpriteRenderer组件 TODO:后期更换为Mesh组件

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