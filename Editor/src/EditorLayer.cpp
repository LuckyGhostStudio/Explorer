#include "EditorLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Explorer/Scene/SceneSerializer.h"
#include "Explorer/Utils/PlatformUtils.h"
#include "Explorer/Utils/ModelImporter.h"

#include "ImGuizmo.h"
#include "Explorer/Math/Math.h"

namespace Explorer
{
	extern const std::filesystem::path g_AssetPath;	//资产目录（全局）

	EditorLayer::EditorLayer() :Layer("EditorLayer")
	{
		
	}

	void EditorLayer::OnAttach()
	{
		m_SelectionIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/ToolBar/SelectionButton.png");		//Selection按钮图标

		m_TranslationIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/ToolBar/TranslationButton.png");	//Translation按钮图标
		m_RotationIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/ToolBar/RotationButton.png");			//Rotation按钮图标
		m_ScaleIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/ToolBar/ScaleButton.png");				//Scale按钮图标

		m_PlayIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/ToolBar/PlayButton.png");					//Play按钮图标

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

		m_MainCamera = m_ActiveScene->CreateCameraObject("Main Camera", true);	//创建默认Camera对象：主相机
		m_Light = m_ActiveScene->CreateLightObject();							//创建默认Light对象（Point）
		m_Cube = m_ActiveScene->CreateMeshObject("Cube", Mesh::Type::Cube);		//创建默认Cube对象

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
		m_SceneHierarchyPanel.SetScene(m_ActiveScene);	//设置Hierarchy面板的场景
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

		//Renderer
		Renderer3D::ResetStats();	//重置统计数据

		m_Framebuffer->Bind();										//绑定帧缓冲区

		glm::vec4& defaultClearColor = m_ActiveScene->GetEnvironment().GetNoneSkyboxColor();	//默认清屏颜色:天空盒缺失默认颜色

		//场景状态
		switch (m_SceneState)
		{
			case SceneState::Edit:	//编辑状态
				RenderCommand::SetClearColor(defaultClearColor);	//设置清屏颜色:天空盒缺失默认颜色
				RenderCommand::Clear();								//清除
				m_Framebuffer->ClearAttachment(1, -1);				//清除颜色缓冲区1（物体id缓冲区）为 -1

				m_EditorCamera.OnUpdate(dt);						//更新编辑器相机
				m_ActiveScene->OnUpdateEditor(dt, m_EditorCamera);	//更新编辑器场景
				break;
			case SceneState::Play:	//运行状态
				Camera& mainCamera = m_ActiveScene->GetPrimaryCameraObject().GetComponent<Camera>();	//场景主相机
				glm::vec4& clearColor = mainCamera.GetClearFlag() == Camera::ClearFlag::Color ? mainCamera.GetBackgroundColor() : defaultClearColor;	//清屏颜色
				RenderCommand::SetClearColor(clearColor);			//设置清屏颜色:场景主相机背景色
				RenderCommand::Clear();								//清除
				m_Framebuffer->ClearAttachment(1, -1);				//清除颜色缓冲区1（物体id缓冲区）为 -1

				m_ActiveScene->OnUpdate(dt);						//运行时场景更新
				break;
		}

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
			EXP_CORE_WARN("pixelData:{0}", pixelData);
			//EXP_CORE_WARN("mx:{0}, my:{1}", mouseX, mouseY);
		}

		//Delete键按下
		if (Input::IsKeyPressed(Key::Delete)) {
			//选中物体存在
			if (m_SceneHierarchyPanel.GetSelectedObject()) {
				Object& object = m_SceneHierarchyPanel.GetSelectedObject();
				m_ActiveScene->DestroyObject(object);			//删除选中物体
				m_SceneHierarchyPanel.SetSelectedObject({});	//选中物体设置为空
			}
		}

		m_Framebuffer->Unbind();	//解除绑定帧缓冲区
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

		// 停靠空间设置
		ImGuiIO& io = ImGui::GetIO();
		ImGuiStyle& style = ImGui::GetStyle();	//样式
		float minWinSizeX = style.WindowMinSize.x;	//最小窗口大小
		style.WindowMinSize.x = 420.0f;

		if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
		{
			ImGuiID dockspace_id = ImGui::GetID("MyDockSpace");
			ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), dockspace_flags);
		}
		style.WindowMinSize.x = minWinSizeX;
		style.FrameRounding = 4.0f;				//控件边框圆度
		style.WindowRounding = 4.0f;			//窗口边框圆度
		style.GrabRounding = 4.0f;				//拖动条handle圆度
		style.PopupRounding = 4.0f;				//弹出窗口圆度

		style.FrameBorderSize = 1.0f;			//边框尺寸
		style.WindowMenuButtonPosition = -1;	//窗口tabbar按钮取消显示
		style.ButtonTextAlign = { 0.5f, 0.5f };	//按钮文字居中

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

				ImGui::Separator();
				//导入3D物体
				if (ImGui::BeginMenu("Import")) {
					//.obj文件
					if (ImGui::MenuItem("Wavefront(.obj)")) {
						ImportModelFile();	//导入模型文件
					}
					ImGui::EndMenu();
				}

				//导出场景物体
				if (ImGui::BeginMenu("Export")) {
					//.obj文件
					if (ImGui::MenuItem("Wavefront(.obj)")) {
						//TODO:导出为.obj文件
					}
					ImGui::EndMenu();
				}
				ImGui::Separator();

				//退出
				if (ImGui::MenuItem("Quit")) {
					Application::GetInstance().Close();	//退出程序
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		m_SceneHierarchyPanel.OnImGuiRender();	//渲染Hierarchy面板
		m_ContentBrowserPanel.OnImGuiRender();	//渲染Project面板

		//批渲染数据统计
		ImGui::Begin("Renderer Stats");
		if (ImGui::BeginMenuBar()) {

			ImGui::EndMenuBar();
		}
		//std::string name = m_PickedObject ? m_PickedObject.GetComponent<Self>().GetObjectName() : "None";
		//ImGui::Text("Hovered Object: %s", name.c_str());

		auto stats = Renderer3D::GetStats();
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Triangle Count: %d", stats.TriangleCount);
		ImGui::Text("Vertex Count (Real): %d", stats.VertexCount);
		ImGui::Text("Index Count: %d", stats.IndexCount);
		ImGui::Text("FPS: %.3f", Application::GetInstance().GetFPS());	//帧率
		
		ImGui::End();	//Renderer Stats

		//场景视口
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));	//设置Gui窗口样式：边界=0
		ImGui::Begin("Scene");
		
		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();	//视口可用区域最小值（视口左上角相对于视口左上角位置）
		//viewportMinRegion.y += 34;	//TODO:向下偏移工具栏的高度
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();	//视口可用区域最大值（视口右下角相对于视口左上角位置）
		auto viewportOffset = ImGui::GetWindowPos();	//视口偏移量：视口面板左上角位置（相对于屏幕左上角）

		m_ViewportBounds[0] = { viewportMinRegion.x + viewportOffset.x, viewportMinRegion.y + viewportOffset.y };
		m_ViewportBounds[1] = { viewportMaxRegion.x + viewportOffset.x, viewportMaxRegion.y + viewportOffset.y };

		m_ViewportFocused = ImGui::IsWindowFocused();	//当前窗口被聚焦
		m_ViewportHovered = ImGui::IsWindowHovered();	//鼠标悬停在当前窗口

		Application::GetInstance().GetImGuiLayer()->BlockEvents(/*!m_ViewportFocused && */!m_ViewportHovered);	//阻止ImGui事件

		ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();			//Scene面板可用区域大小
		m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };		//视口大小

		uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();	//颜色缓冲区0 ID
		ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));	//场景视口Image
		ImGui::SetItemAllowOverlap();	//允许覆盖

		//将从拖拽源（Project面板）复制的数据拖放到目标（场景）	：场景路径
		ContentBrowserPanel::DragDropToTarget({ ".explor", ".obj", ".mesh" }, [&](const std::filesystem::path& filepath)
		{
			const std::string filename = filepath.filename().string();	//文件名
			uint32_t dotIndex = filename.find_last_of('.');
			std::string suffixname = filename.substr(dotIndex, filename.length() - dotIndex);	//文件后缀名

			if (suffixname == ".explor") {	//场景文件
				OpenScene(filepath);		//打开场景
			}
			else if (suffixname == ".obj" || suffixname == ".mesh") {	//.obj模型文件 or .mesh文件（实际也是.obj）
				ImportModelFile(filepath);		//导入模型文件
			}
		});

		//设置工具栏
		ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
		UI_ToolBar();	//工具栏
		ImGui::SetItemAllowOverlap();

		//编辑器相机
		const glm::mat4& cameraProjection = m_EditorCamera.GetProjectionMatrix();	//投影矩阵
		const glm::mat4& cameraView = m_EditorCamera.GetViewMatrix();				//视图矩阵
		
		Gizmo::Init(m_ViewportBounds[0], m_ViewportBounds[1]);												//初始化Gizmo绘制参数	
		Gizmo::DrawTransformation(m_SceneHierarchyPanel.GetSelectedObject(), cameraView, cameraProjection);	//绘制Transform Gizmos
		
		ImGui::End();	//Scene
		ImGui::PopStyleVar();

		//ImGui::ShowDemoWindow();	//样例窗口
		
		ImGui::End();	//DockSpace
	}

	void EditorLayer::UI_ToolBar()
	{
		float buttonSize = 20.0f;

		static bool buttonHovereds[5] = { false, false, false, false, false };	//按钮是否被悬浮

		m_Pickable = !(buttonHovereds[0] | buttonHovereds[1] | buttonHovereds[2] | buttonHovereds[3] | buttonHovereds[4]);	//可拾取：都没有被悬浮

		//按钮颜色
		static ImVec4 buttonColors[4] = { 
			{ 0.14f, 0.29f, 0.42f, 1.0f },	//Selection按钮颜色 默认选中
			{ 0.2f, 0.205f, 0.21f, 1.0f },	//Translation按钮颜色
			{ 0.2f, 0.205f, 0.21f, 1.0f },	//Rotation按钮颜色
			{ 0.2f, 0.205f, 0.21f, 1.0f }	//Scale按钮颜色
		};	
		
		for (int i = 0; i < 4; i++) {
			int selected = int(Gizmo::s_TransformType) + 1;	//已选中的操作
			if (i != selected) {
				buttonColors[i] = { 0.2f, 0.205f, 0.21f, 1.0f };	//原色
			}
			else {
				buttonColors[i] = { 0.14f, 0.29f, 0.42f, 1.0f };	//蓝色
			}
		}
		

		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMin().x + 4.0f);
		ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMin().y + 4.0f);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, buttonColors[0]);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { buttonColors[0].x + 0.01f, buttonColors[0].y + 0.01f, buttonColors[0].z + 0.01f, 1.0f });
		
		//Selection按钮 按下
		if (ImGui::ImageButton((ImTextureID)m_SelectionIcon->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 1), ImVec2(1, 0))) {
			Gizmo::s_TransformType = Gizmo::TransformationType::None;	//变换类型 ：None
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);

		//此按钮被悬浮
		if (ImGui::IsItemHovered()) {
			buttonHovereds[0] = true;	//被悬浮
		}
		else {
			buttonHovereds[0] = false;
		}

		//Translation按钮------------------------------------------
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMin().x + 4.0f * 2 + 4.0f + buttonSize);
		ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMin().y + 4.0f);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, buttonColors[1]);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { buttonColors[1].x + 0.01f, buttonColors[1].y + 0.01f, buttonColors[1].z + 0.01f, 1.0f });

		//Translation按钮 按下
		if (ImGui::ImageButton((ImTextureID)m_TranslationIcon->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 1), ImVec2(1, 0))) {
			Gizmo::s_TransformType = Gizmo::TransformationType::Translate;	//平移
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);

		//此按钮被悬浮
		if (ImGui::IsItemHovered()) {
			buttonHovereds[1] = true;	//被悬浮
		}
		else {
			buttonHovereds[1] = false;
		}

		//Rotation按钮------------------------------------------
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMin().x + 4.0f * 3 + 4.0f * 2 + buttonSize * 2);
		ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMin().y + 4.0f);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, buttonColors[2]);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { buttonColors[2].x + 0.01f, buttonColors[2].y + 0.01f, buttonColors[2].z + 0.01f, 1.0f });

		//Rotation按钮 按下
		if (ImGui::ImageButton((ImTextureID)m_RotationIcon->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 1), ImVec2(1, 0))) {
			Gizmo::s_TransformType = Gizmo::TransformationType::Rotate;	//旋转
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);

		//此按钮被悬浮
		if (ImGui::IsItemHovered()) {
			buttonHovereds[2] = true;	//被悬浮
		}
		else {
			buttonHovereds[2] = false;
		}

		//Scale按钮------------------------------------------
		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMin().x + 4.0f * 4 + 4.0f * 3 + buttonSize * 3);
		ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMin().y + 4.0f);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, buttonColors[3]);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { buttonColors[3].x + 0.01f, buttonColors[3].y + 0.01f, buttonColors[3].z + 0.01f, 1.0f });

		//Scale按钮 按下
		if (ImGui::ImageButton((ImTextureID)m_ScaleIcon->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 1), ImVec2(1, 0))) {
			Gizmo::s_TransformType = Gizmo::TransformationType::Scale;	//缩放
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);

		//此按钮被悬浮
		if (ImGui::IsItemHovered()) {
			buttonHovereds[3] = true;	//被悬浮
		}
		else {
			buttonHovereds[3] = false;
		}

		//Play按钮设置-------------------------------
		static ImVec4 playButtonColor = { 0.2f, 0.205f, 0.21f, 1.0f };	//Play按钮颜色

		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x * 0.5f - buttonSize * 0.5f);	//按钮居中
		ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMin().y + 4.0f);
		
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, playButtonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { playButtonColor.x + 0.01f, playButtonColor.y + 0.01f, playButtonColor.z + 0.01f, 1.0f });
		//Play按钮 按下
		if (ImGui::ImageButton((ImTextureID)m_PlayIcon->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 0), ImVec2(1, 1))) {
			//场景状态
			switch (m_SceneState)
			{
				case SceneState::Edit:	//编辑状态
					playButtonColor = { 0.14f, 0.29f, 0.42f, 1.0f };	//蓝色
					OnScenePlay();
					break;
				case SceneState::Play:	//运行状态
					playButtonColor = { 0.2f, 0.205f, 0.21f, 1.0f };
					OnSceneStop();
					break;
			}
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);

		//此按钮被悬浮
		if (ImGui::IsItemHovered()) {
			buttonHovereds[4] = true;	//被悬浮
		}
		else {
			buttonHovereds[4] = false;
		}
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_EditorCamera.OnEvent(event);	//编辑器相机事件

		EventDispatcher dispatcher(event);
		dispatcher.Dispatch<KeyPressedEvent>(EXP_BIND_EVENT_FUNC(EditorLayer::OnKeyPressed));					//调用按键按下事件TODO:焦点在场景窗口时才触发
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
		switch (e.GetKeyCode()) 
		{
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
			case Key::W:
				Gizmo::s_TransformType = Gizmo::TransformationType::None;		//选择
				break;
			case Key::G:
				Gizmo::s_TransformType = Gizmo::TransformationType::Translate;	//平移
				break;
			case Key::R:
				Gizmo::s_TransformType = Gizmo::TransformationType::Rotate;		//旋转
				break;
			case Key::S:
				Gizmo::s_TransformType = Gizmo::TransformationType::Scale;		//缩放
				break;
		}
	}

	bool EditorLayer::OnMouseButtonPressed(MouseButtonPressedEvent& e)
	{
		//鼠标左键按下
		if (e.GetMouseButton() == Mouse::ButtonLeft) {
			//鼠标在视口内 Gizmo控制没结束 可拾取
			if (m_ViewportHovered && !ImGuizmo::IsOver() && m_Pickable) {
				m_SceneHierarchyPanel.SetSelectedObject(m_PickedObject);	//设置被选中物体
			}
		}

		return false;
	}

	void EditorLayer::NewScene()
	{
		m_ActiveScene = std::make_shared<Scene>("New Scene");			//创建新场景

		m_MainCamera = m_ActiveScene->CreateCameraObject("Main Camera", true);	//创建默认Camera对象：主相机
		m_Light = m_ActiveScene->CreateLightObject();							//创建默认Light对象
		m_Cube = m_ActiveScene->CreateMeshObject("Cube", Mesh::Type::Cube);		//创建默认Cube对象

		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//重置视口大小
		m_SceneHierarchyPanel.SetScene(m_ActiveScene);	//设置Hierarchy的场景
	}

	void EditorLayer::OpenScene()
	{
		std::string filepath = FileDialogs::OpenFile("Explorer Scene(*.explor)\0*.explor\0");	//打开文件对话框（文件类型名\0 文件类型.explor）
		//路径不为空
		if (!filepath.empty()) {
			OpenScene(filepath);	//打开场景
		}
	}

	void EditorLayer::OpenScene(const std::filesystem::path& path)
	{
		m_ActiveScene = std::make_shared<Scene>();		//创建新场景
		m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//重置视口大小
		m_SceneHierarchyPanel.SetScene(m_ActiveScene);	//设置Hierarchy的场景

		SceneSerializer serializer(m_ActiveScene);	//场景序列化器
		serializer.Deserialize(path.string());		//反序列化：加载文件场景到新场景
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Explorer Scene(*.explor)\0*.explor\0");	//保存文件对话框（文件类型名\0 文件类型.explor）

		//路径不为空
		if (!filepath.empty()) {
			std::string sceneName = std::filesystem::path(filepath).stem().string();	//场景名（场景文件名）
			m_ActiveScene->SetName(sceneName);

			SceneSerializer serializer(m_ActiveScene);	//场景序列化器
			serializer.Serialize(filepath);				//序列化：保存场景
		}
	}

	void EditorLayer::ImportModelFile()
	{
		std::string filepath = FileDialogs::OpenFile("Wavefront(*.obj)\0*.obj\0");	//打开文件对话框 .obj文件
		//路径不为空
		if (!filepath.empty()) {
			ImportModelFile(filepath);	//导入模型文件
		}
	}

	void EditorLayer::ImportModelFile(const std::filesystem::path& path)
	{
		ModelImporter::Load(path.string());		//加载文件

		auto& mesh = ModelImporter::GetMesh();	//已导入的Mesh

		std::string& name = path.stem().string();	//物体名/网格名

		Object object = m_ActiveScene->CreateEmptyObject(name);	//创建空物体
			
		mesh.SetType(Mesh::Type::Other);	//Mesh类型：其他网格
		mesh.SetName(name);					//设置Mesh名

		object.AddComponent<Mesh>(mesh);	//添加Mesh组件
		object.AddComponent<Material>();	//添加Material组件
	}

	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;	//开始运行
	}

	void EditorLayer::OnSceneStop()
	{
		m_SceneState = SceneState::Edit;	//停止运行
	}
}