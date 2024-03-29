#include "EditorLayer.h"
#include "imgui/imgui.h"
#include "imgui/imgui_internal.h"

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
		m_GizmosIcon = std::make_shared<Texture2D>("Resources/Icons/Buttons/ToolBar/GizmosButton.png");				//Gizmos按钮图标
		//0.886f, 0.376f, 0.357f, 0.702f
		m_CoordinateAxis[0] = Line({ -20, 0, 0 }, { 20, 0, 0 }, { 0.815f, 0.175f, 0.216f, 1.0f });	//x轴
		m_CoordinateAxis[1] = Line({ 0, 0, -20 }, { 0, 0, 20 }, { 0.025f, 0.275f, 0.68f, 1.0f });	//z轴

		glm::vec4 gridColor = { 0.4f, 0.4f, 0.4f, 0.302f };

		for (int i = -20; i < 0; i++) {
			m_AxisGrids[i + 20].SetWidth(1.0f);
			m_AxisGrids[i + 20] = Line({ i, 0, -20 }, { i, 0, 20 }, gridColor);	//x-方向
			m_AxisGrids[i + 40].SetWidth(1.0f);
			m_AxisGrids[i + 40] = Line({ -20, 0, i }, { 20, 0, i }, gridColor);	//z-方向
		}
		for (int i = 0; i < 20; i++) {
			m_AxisGrids[i + 40].SetWidth(1.0f);
			m_AxisGrids[i + 40] = Line({ i + 1, 0, -20 }, { i + 1, 0, 20 }, gridColor);	//x+方向
			m_AxisGrids[i + 60].SetWidth(1.0f);
			m_AxisGrids[i + 60] = Line({ -20, 0, i + 1 }, { 20, 0, i + 1 }, gridColor);	//z+方向
		}

		FramebufferSpecification fbSpec;	//帧缓冲区规范
		fbSpec.Attachments = {
			FramebufferTextureFormat::RGBA8,		//颜色缓冲区0格式
			FramebufferTextureFormat::RED_INTEGER,	//颜色缓冲区1格式：作为id实现鼠标点击拾取
			FramebufferTextureFormat::Depth			//深度缓冲区格式
		};
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = std::make_shared<Framebuffer>(fbSpec);	//创建帧缓冲区

		m_EditorScene = std::make_shared<Scene>("New Scene");					//创建场景
		m_EditorCamera = EditorCamera(glm::radians(60.0f), 1280.0f / 720.0f, 0.01f, 1000.0f);	//创建编辑器相机

		m_MainCamera = m_EditorScene->CreateCameraObject("Main Camera", true);	//创建默认Camera对象：主相机
		m_Light = m_EditorScene->CreateLightObject();							//创建默认Light对象（Point）
		m_Cube = m_EditorScene->CreateMeshObject("Cube", Mesh::Type::Cube);		//创建默认Cube对象

		m_SceneHierarchyPanel.SetScene(m_EditorScene);	//设置Hierarchy面板的场景

		m_ActiveScene = m_EditorScene;
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
			//EXP_CORE_WARN("pixelData:{0}", pixelData);
			//EXP_CORE_WARN("mx:{0}, my:{1}", mouseX, mouseY);
		}

		OnOverlayRender();	//渲染覆盖层：Gizmo

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
		style.FrameRounding = 3.0f;				//控件边框圆度
		style.WindowRounding = 4.0f;			//窗口边框圆度
		style.GrabRounding = 4.0f;				//拖动条handle圆度
		style.PopupRounding = 4.0f;				//弹出窗口圆度

		style.FrameBorderSize = 1.0f;			//边框尺寸
		style.WindowMenuButtonPosition = -1;	//窗口tabbar按钮取消显示
		style.ButtonTextAlign = { 0.5f, 0.5f };	//按钮文字居中

		UI_MenuBar();			//菜单栏
		UI_PreferencesPanel();	//偏好设置面板
		UI_RenderingPanel();	//渲染设置面板

		m_SceneHierarchyPanel.OnImGuiRender();	//渲染Hierarchy面板
		m_ContentBrowserPanel.OnImGuiRender();	//渲染Project面板

		//批渲染数据统计
		ImGui::Begin("Renderer Stats");
		//std::string name = m_PickedObject ? m_PickedObject.GetComponent<Self>().GetObjectName() : "None";
		//ImGui::Text("Hovered Object: %s", name.c_str());

		auto stats = Renderer3D::GetStats();
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Triangle Count: %d", stats.TriangleCount);
		ImGui::Text("Vertex Count (Real): %d", stats.VertexCount);
		ImGui::Text("Index Count: %d", stats.IndexCount);
		ImGui::Text("FPS: %.3f", Application::GetInstance().GetFPS());	//帧率
		
		ImGui::End();	//Renderer Stats

		UI_SceneViewportPanel();	//场景视口面板

		//ImGui::ShowDemoWindow();	//样例窗口
		
		ImGui::End();	//DockSpace
	}

	void EditorLayer::UI_MenuBar()
	{
		//菜单条
		if (ImGui::BeginMenuBar()) {
			//菜单：File
			if (ImGui::BeginMenu("File")) {
				//创建新场景
				if (ImGui::MenuItem("New", "Ctrl N")) {
					NewScene();
				}

				//打开文件：加载场景
				if (ImGui::MenuItem("Open...", "Ctrl O")) {
					OpenScene();
				}

				//保存场景
				if (ImGui::MenuItem("Save", "Ctrl S")) {
					SaveScene();
				}

				//另存为：保存场景
				if (ImGui::MenuItem("Save As...", "Ctrl Shift S")) {
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
				if (ImGui::BeginMenu("Export", false)) {
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
					//TODO:退出时询问是否保存该场景
				}
				ImGui::EndMenu();
			}
			//菜单：Edit
			if (ImGui::BeginMenu("Edit")) {
				//偏好设置窗口
				ImGui::MenuItem("Preferences", nullptr, &m_PreferencesWindowOpened);

				ImGui::EndMenu();
			}
			//菜单：Window
			if (ImGui::BeginMenu("Window")) {
				//渲染设置窗口
				ImGui::MenuItem("Rendering", nullptr, &m_RenderingWindowOpened);

				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}
	}

	void EditorLayer::UI_ToolBar()
	{
		float buttonSize = 20.0f;

		static bool buttonHovereds[6] = { false, false, false, false, false, false };	//按钮是否被悬浮

		m_Pickable = !(buttonHovereds[0] | buttonHovereds[1] | buttonHovereds[2] | buttonHovereds[3] | buttonHovereds[4] | buttonHovereds[5]);	//可拾取：都没有被悬浮

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

		//Selection按钮------------------------------------------
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

		//Gizmos按钮设置-------------------------------
		static ImVec4 gizmosButtonColor = { 0.2f, 0.205f, 0.21f, 1.0f };	//Gizmos按钮颜色

		if (m_ShowGizmos) gizmosButtonColor = { 0.14f, 0.29f, 0.42f, 1.0f };	//蓝色
		else gizmosButtonColor = { 0.2f, 0.205f, 0.21f, 1.0f };

		ImGui::SetCursorPosX(ImGui::GetWindowContentRegionMax().x - buttonSize * 2.0f);
		ImGui::SetCursorPosY(ImGui::GetWindowContentRegionMin().y + 4.0f);

		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 2));
		ImGui::PushStyleVar(ImGuiStyleVar_ItemInnerSpacing, ImVec2(0, 0));
		ImGui::PushStyleColor(ImGuiCol_Button, gizmosButtonColor);
		ImGui::PushStyleColor(ImGuiCol_ButtonHovered, { gizmosButtonColor.x + 0.01f, gizmosButtonColor.y + 0.01f, gizmosButtonColor.z + 0.01f, 1.0f });
		//Gizmos按钮 按下
		if (ImGui::ImageButton((ImTextureID)m_GizmosIcon->GetRendererID(), ImVec2(buttonSize, buttonSize), ImVec2(0, 1), ImVec2(1, 0))) {
			m_ShowGizmos = !m_ShowGizmos;	//设置Gizmos显示状态
		}
		ImGui::PopStyleVar(2);
		ImGui::PopStyleColor(2);

		//此按钮被悬浮
		if (ImGui::IsItemHovered()) {
			buttonHovereds[5] = true;	//被悬浮
		}
		else {
			buttonHovereds[5] = false;
		}
	}

	void EditorLayer::UI_SceneViewportPanel()
	{
		//场景视口
		ImGui::PushStyleVar(ImGuiStyleVar_WindowPadding, ImVec2(0, 0));	//设置Gui窗口样式：边界=0
		ImGui::Begin("Scene");

		auto viewportMinRegion = ImGui::GetWindowContentRegionMin();	//视口可用区域最小值（视口左上角相对于视口左上角位置）
		auto viewportMaxRegion = ImGui::GetWindowContentRegionMax();	//视口可用区域最大值（视口右下角相对于视口左上角位置）
		auto viewportOffset = ImGui::GetWindowPos();					//视口偏移量：视口面板左上角位置（相对于屏幕左上角）

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
			const std::string extension = filepath.extension().string();	//扩展名

			if (extension == ".explor") {	//场景文件
				OpenScene(filepath);		//打开场景
			}
			else if (extension == ".obj" || extension == ".mesh") {	//.obj模型文件 or .mesh文件（实际也是.obj）
				ImportModelFile(filepath);		//导入模型文件
			}
		});

		//设置工具栏
		ImGui::SetCursorPos(ImGui::GetWindowContentRegionMin());
		UI_ToolBar();	//工具栏
		ImGui::SetItemAllowOverlap();

		//编辑状态
		if (m_SceneState == SceneState::Edit) {
			//初始化Gizmo绘制参数	
			Gizmo::Init(m_ViewportBounds[0], m_ViewportBounds[1]);
			//绘制Transform Gizmos
			Gizmo::DrawTransformation(m_SceneHierarchyPanel.GetSelectedObject(), m_EditorCamera.GetViewMatrix(), m_EditorCamera.GetProjectionMatrix());
		}

		ImGui::End();	//Scene
		ImGui::PopStyleVar();
	}

	void EditorLayer::UI_PreferencesPanel()
	{
		if (!m_PreferencesWindowOpened) return;

		//偏好设置面板
		ImGui::Begin("Preferences", &m_PreferencesWindowOpened);

		//EditorCamera设置 树节点
		UI::DrawTreeNode<EditorCamera>("Editor Camera", true, [&](float lineHeight)
		{
			UI::DrawSlider("Vertical FOV", &m_EditorCamera.GetFOV_Ref(), 4.0f, 120.0f, UI::ValueType::Angle);	//FOV垂直张角 滑动条
			UI::DrawDrag("Near", &m_EditorCamera.GetNearClip_Ref(), 0.01f, UI::ValueType::Float, 0.01f, m_EditorCamera.GetFarClip() - 0.01f);	//Near近裁剪平面 拖动条
			UI::DrawDrag("Far", &m_EditorCamera.GetFarClip_Ref(), 0.01f, UI::ValueType::Float, m_EditorCamera.GetNearClip() + 0.01f, 1000.0f);	//Far远裁剪平面 拖动条
			
			UI::DrawDrag("Rotation Speed", &m_EditorCamera.GetRotationSpeed_Ref(), 0.01f, UI::ValueType::Float, 0.2f, 2.0f);	//旋转速度 拖动条
			UI::DrawDrag("View Zoom Rate", &m_EditorCamera.GetViewZoomRate_Ref(), 0.01f, UI::ValueType::Float, 0.1f, 0.4f);		//视图缩放速率 拖动条
		});

		//Physics2D设置 树节点
		UI::DrawTreeNode<glm::vec2>("Physics 2D", true, [&](float lineHeight)
		{
			UI::DrawDrag("Gravity", glm::value_ptr(m_ActiveScene->GetGravity()), 0.01f, UI::ValueType::Float2);	//重力加速度 拖动条
		});

		ImGui::End();	//Preferences
	}

	void EditorLayer::UI_RenderingPanel()
	{
		if (!m_RenderingWindowOpened) return;

		//渲染器设置面板
		ImGui::Begin("Rendering Settings", &m_RenderingWindowOpened);
		{
			if (ImGui::BeginTabBar("##Tabs", ImGuiTabBarFlags_None)) {
				//渲染器设置
				if (ImGui::BeginTabItem("Renderer")) {
					const char* rendererTypes[] = { "Rasterization", "Ray Tracing" };	//渲染器类型
					const char* currentRenderer = rendererTypes[(int)Renderer::s_Type];	//当前渲染器类型

					//渲染器类型 选择下拉列表
					UI::DrawDropdownList("Render Engine", currentRenderer, rendererTypes, 2, [&](int index, const char* value)
					{
						Renderer::s_Type = (Renderer::Type)index;	//设置渲染器类型
						if (Renderer::s_Type == Renderer::Type::Raytracing) {
							NewScene("Ray Tracing Test Scene", false);	//创建RayTracing测试场景

							Object ground = m_ActiveScene->CreateSphereObjectToRayTracing("Ground");
							ground.GetComponent<Transform>().SetScale({ 100, 100, 100 });
							ground.GetComponent<Transform>().SetPosition({ 0, -50, 0 });
						}
						else {
							m_EditorScenePath.empty() ? NewScene() : OpenScene(m_EditorScenePath);	//创建新场景或打开已存在场景
						}
					});

					ImGui::EndTabItem();
				}

				//环境设置
				if (ImGui::BeginTabItem("Environment")) {
					Environment& environment = m_ActiveScene->GetEnvironment();	//场景环境
					Skybox& skybox = environment.GetSkybox();	//天空盒

					UI::DrawCheckBox("Enable Skybox", &environment.GetSkyboxEnable_Ref());	//Enable Skybox是否启用天空盒 勾选框

					//Skybox天空盒设置 树节点
					UI::DrawTreeNode<Environment>(skybox.GetShader()->GetName() + "(Skybox)", true, [&](float lineHeight)
					{
						UI::DrawColorEditor3("Tint Color", glm::value_ptr(skybox.GetTintColor()));					//Tint Color色调 颜色编辑器
						UI::DrawSlider("Expose", &skybox.GetExpose_Ref(), 0.0f, 8.0f);								//Expose曝光度 滑动条
						UI::DrawSlider("Rotation", &skybox.GetRotation_Ref(), 0.0f, 360.0f, UI::ValueType::Angle);	//Rotation旋转值z 滑动条

						uint32_t rightTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Right]->GetRendererID();	//Right预览贴图ID
						uint32_t leftTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Left]->GetRendererID();	//Left预览贴图ID
						uint32_t upTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Up]->GetRendererID();		//Up预览贴图ID
						uint32_t downTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Down]->GetRendererID();	//Down预览贴图ID
						uint32_t frontTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Front]->GetRendererID();	//Front预览贴图ID
						uint32_t backTextureID = skybox.GetPreviewTextures()[(int)Cubemap::TextureDirection::Back]->GetRendererID();	//Back预览贴图ID

						//Cubemap贴图设置 树节点
						UI::DrawTreeNode<Cubemap>("Cubemap Settings", false, [&](float lineHeight)
						{
							glm::vec2 textureButtonSize = { 50, 50 };

							//Right(+x)天空盒贴图 选择&预览按钮
							UI::DrawImageButton("Right [+X]", rightTextureID, textureButtonSize, [&]()
							{
								std::string filepath = FileDialogs::OpenFile("Right[+X] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//打开文件对话框.png|.jpg
								if (!filepath.empty()) {
									skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Right);	//设置Right预览贴图
								}
							});

							//将从拖拽源（Project面板）复制的数据拖放到目标（Right(+x)）	：文件路径
							ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
							{
								skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Right);	//设置Right预览贴图
							});

							//Left(-x)天空盒贴图 选择&预览按钮
							UI::DrawImageButton("Left [-X]", leftTextureID, textureButtonSize, [&]()
							{
								std::string filepath = FileDialogs::OpenFile("Left[-X] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//打开文件对话框.png|.jpg
								if (!filepath.empty()) {
									skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Left);	//设置Left预览贴图
								}
							});

							//将从拖拽源（Project面板）复制的数据拖放到目标（Left(-x)）	：文件路径
							ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
							{
								skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Left);	//设置Left预览贴图
							});

							//Up(+y)天空盒贴图 选择&预览按钮
							UI::DrawImageButton("Up [+Y]", upTextureID, textureButtonSize, [&]()
							{
								std::string filepath = FileDialogs::OpenFile("Up[+Y] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//打开文件对话框.png|.jpg
								if (!filepath.empty()) {
									skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Up);	//设置Up预览贴图
								}
							});

							//将从拖拽源（Project面板）复制的数据拖放到目标（Up(+y)）	：文件路径
							ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
							{
								skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Up);	//设置Up预览贴图
							});

							//Down(-y)天空盒贴图 选择&预览按钮
							UI::DrawImageButton("Down [-Y]", downTextureID, textureButtonSize, [&]()
							{
								std::string filepath = FileDialogs::OpenFile("Down[-Y] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//打开文件对话框.png|.jpg
								if (!filepath.empty()) {
									skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Down);	//设置Down预览贴图
								}
							});

							//将从拖拽源（Project面板）复制的数据拖放到目标（Down(-y)）	：文件路径
							ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
							{
								skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Down);	//设置Down预览贴图
							});

							//Front(+z)天空盒贴图 选择&预览按钮
							UI::DrawImageButton("Front [+Z]", frontTextureID, textureButtonSize, [&]()
							{
								std::string filepath = FileDialogs::OpenFile("Front[+Z] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//打开文件对话框.png|.jpg
								if (!filepath.empty()) {
									skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Front);	//设置Front预览贴图
								}
							});

							//将从拖拽源（Project面板）复制的数据拖放到目标（Front(+z)）	：文件路径
							ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
							{
								skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Front);	//设置Front预览贴图
							});

							//Back(-z)天空盒贴图 选择&预览按钮
							UI::DrawImageButton("Back [-Z]", backTextureID, textureButtonSize, [&]()
							{
								std::string filepath = FileDialogs::OpenFile("Back[-Z] Texture(*.png;*.jpg)\0*.png;*.jpg\0");	//打开文件对话框.png|.jpg
								if (!filepath.empty()) {
									skybox.SetCubemapOneSideTexture(filepath, Cubemap::TextureDirection::Back);	//设置Back预览贴图
								}
							});

							//将从拖拽源（Project面板）复制的数据拖放到目标（Back(-z)）	：文件路径
							ContentBrowserPanel::DragDropToTarget({ ".jpg" }, [&](const std::filesystem::path& filepath)
							{
								skybox.SetCubemapOneSideTexture(filepath.string(), Cubemap::TextureDirection::Back);	//设置Back预览贴图
							});
						}, false);

						//Cubemap预览图 树节点
						UI::DrawTreeNode<Cubemap>("Cubemap Preview", false, [&](float lineHeight)
						{
							ImVec2 size = ImVec2(90, 90);	//Cubemap单张大小
							int positionX = 35;				//Cubemap X位置

							//Cubemap预览图
							ImGui::SetCursorPosX(positionX);
							ImGui::PushStyleVar(ImGuiStyleVar_ChildBorderSize, 0);
							ImGui::BeginChild("Cubemap Preview", ImVec2(size.x * 4 + 40, size.y * 3 + 40), true, ImGuiWindowFlags_NoScrollbar | ImGuiWindowFlags_NoScrollWithMouse);

							ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
							//Up [+Y]
							ImGui::SetCursorPosX(size.x);	//设置游标X位置
							ImGui::Image((void*)upTextureID, size);
							//Left [-X]
							ImGui::SetCursorPosX(0);
							ImGui::Image((void*)leftTextureID, size, ImVec2(1, 1), ImVec2(0, 0));
							ImGui::SameLine();
							//Back [-Z]
							ImGui::SetCursorPosX(size.x);
							ImGui::Image((void*)backTextureID, size, ImVec2(1, 1), ImVec2(0, 0));
							ImGui::SameLine();
							//Right [+X]
							ImGui::SetCursorPosX(size.x * 2);
							ImGui::Image((void*)rightTextureID, size, ImVec2(1, 1), ImVec2(0, 0));
							ImGui::SameLine();
							//Front [+Z]
							ImGui::SetCursorPosX(size.x * 3);
							ImGui::Image((void*)frontTextureID, size, ImVec2(1, 1), ImVec2(0, 0));
							//Down [-Y]
							ImGui::SetCursorPosX(size.x);
							ImGui::Image((void*)downTextureID, size);

							ImGui::PopStyleVar();

							ImGui::EndChild();
							ImGui::PopStyleVar();
						}, false);
					});

					//Environment Lighting环境光照设置 树节点
					UI::DrawTreeNode<Environment>("Environment Lighting", true, [&](float lineHeight)
					{
						const char* sourceTypes[] = { "Skybox","Color" };							//环境光源类型
						const char* currentSource = sourceTypes[(int)environment.GetSourceType()];	//当前环境光源类型

						//Source环境光源类型 选择下拉列表
						UI::DrawDropdownList("Source", currentSource, sourceTypes, 2, [&](int index, const char* value)
						{
							environment.SetSourceType((Environment::SourceType)index);	//设置环境光源类型
						});

						//天空盒未启用
						if (!environment.GetSkyboxEnable()) {
							UI::DrawColorEditor3("Ambient Color", glm::value_ptr(environment.GetAmbientColor()));	//AmbientColor环境光颜色 颜色编辑器
						}
						else {
							//环境光源 天空盒（不叠加光源为颜色的环境光）
							if (environment.GetSourceType() == Environment::SourceType::Skybox) {
								UI::DrawSlider("Intensity Multiplier", &environment.GetIntensityMultiplier_Ref(), 0.0f, 8.0f, UI::ValueType::Float);	//光强度倍数 滑动条
							}
							//环境光源为 颜色
							if (environment.GetSourceType() == Environment::SourceType::Color) {
								UI::DrawColorEditor3("Ambient Color", glm::value_ptr(environment.GetAmbientColor()));	//AmbientColor环境光颜色 颜色编辑器
							}
						}

					});

					ImGui::EndTabItem();
				}

				ImGui::EndTabBar();
			}
		}
		ImGui::End();	//Environment
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

		switch (e.GetKeyCode()) 
		{
			//菜单快捷键
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
				if (control) {
					if (shift) SaveSceneAs();	//场景另存为：Ctrl+Shift+S
					else SaveScene();			//保存场景：Ctrl+S
				}
				break;
			//复制物体 ctrl+D
			case Key::D:
				if (control) {
					OnCopyObject();	//复制物体
				}
				break;
			case Key::Delete:
				OnDeleteObject();	//复制物体
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

	void EditorLayer::OnOverlayRender()
	{
		if (Renderer::s_Type == Renderer::Type::Raytracing) return;

		if (!m_ShowGizmos) return;	//不显示Gizmos

		if (m_SceneState == SceneState::Play) return;

		Renderer3D::BeginScene(m_EditorCamera);

		//================AxisGrids=========================
		Renderer3D::DrawLine(m_CoordinateAxis[0]);	//绘制x轴
		Renderer3D::DrawLine(m_CoordinateAxis[1]);	//绘制z轴
		//绘制坐标格
		for (int i = 0; i < 80; i++) {
			Renderer3D::DrawLine(m_AxisGrids[i]);
		}

		Object selectedObject = m_SceneHierarchyPanel.GetSelectedObject();	//选中项

		//========================Components==================================
		//绘制Camera Gizmo
		{
			auto view = m_ActiveScene->GetAllObjectsWith<Transform, Camera>();
			for (auto object : view) {
				auto [transform, camera] = view.get<Transform, Camera>(object);
				Object obj = { object, m_ActiveScene.get() };
				//object启用 && 被选中
				if (obj.GetEnable() && selectedObject == obj) {
					glm::vec4 p[8];	//相机边界位置
					float aspectRatio = camera.GetAspectRatio();
					float nearClip = camera.GetNearClip();
					float farClip = camera.GetFarClip();
					glm::mat4 trans = transform.GetTransform();
					//透视投影
					if (camera.GetProjectionType() == Camera::ProjectionType::Orthographic) {
						float size = camera.GetSize();
						
						p[0] = trans * glm::vec4(-size * aspectRatio, -size, -nearClip, 1.0f);
						p[1] = trans * glm::vec4(size * aspectRatio, -size, -nearClip, 1.0f);
						p[2] = trans * glm::vec4(size * aspectRatio, size, -nearClip, 1.0f);
						p[3] = trans * glm::vec4(-size * aspectRatio, size, -nearClip, 1.0f);

						p[4] = trans * glm::vec4(-size * aspectRatio, -size, - farClip, 1.0f);
						p[5] = trans * glm::vec4(size * aspectRatio, -size, - farClip, 1.0f);
						p[6] = trans * glm::vec4(size * aspectRatio, size, - farClip, 1.0f);
						p[7] = trans * glm::vec4(-size * aspectRatio, size, - farClip, 1.0f);
					}
					else {
						float fov = camera.GetFOV();
						float tanHalfFOV = glm::tan(fov / 2);

						p[0] = trans * glm::vec4(-nearClip * tanHalfFOV * aspectRatio, -nearClip * tanHalfFOV, -nearClip, 1.0f);
						p[1] = trans * glm::vec4(nearClip * tanHalfFOV * aspectRatio, -nearClip * tanHalfFOV, -nearClip, 1.0f);
						p[2] = trans * glm::vec4(nearClip * tanHalfFOV * aspectRatio, nearClip * tanHalfFOV, -nearClip, 1.0f);
						p[3] = trans * glm::vec4(-nearClip * tanHalfFOV * aspectRatio, nearClip * tanHalfFOV, -nearClip, 1.0f);

						p[4] = trans * glm::vec4(-farClip * tanHalfFOV * aspectRatio, -farClip * tanHalfFOV, -farClip, 1.0f);
						p[5] = trans * glm::vec4(farClip * tanHalfFOV * aspectRatio, -farClip * tanHalfFOV, -farClip, 1.0f);
						p[6] = trans * glm::vec4(farClip * tanHalfFOV * aspectRatio, farClip * tanHalfFOV, -farClip, 1.0f);
						p[7] = trans * glm::vec4(-farClip * tanHalfFOV * aspectRatio, farClip * tanHalfFOV, -farClip, 1.0f);
					}
					//绘制Camera Gizmo
					for (int i = 0; i < 4; i++) {
						Renderer3D::DrawLine(p[i], p[(i + 1) % 4], { 1, 1, 1, 1 });			//绘制近裁剪面矩形
						Renderer3D::DrawLine(p[i], p[i + 4], { 1, 1, 1, 1 });				//绘制中间连接线
						Renderer3D::DrawLine(p[i + 4], p[(i + 5) % 4 + 4], { 1, 1, 1, 1 });	//绘制远裁剪面矩形
					}
				}
			}
		}

		//绘制Light Gizmo
		{
			auto view = m_ActiveScene->GetAllObjectsWith<Transform, Light>();
			for (auto object : view) {
				auto [transform, light] = view.get<Transform, Light>(object);
				Object obj = { object, m_ActiveScene.get() };
				//object启用 && 被选中
				if (obj.GetEnable() && selectedObject == obj) {
					glm::vec3 position = transform.GetPosition();			//光源位置
					glm::vec3 direction = -transform.GetForwardDirection();	//光照方向
					float range = light.GetRange();
					glm::mat4 trans = glm::translate(glm::mat4(1.0f), transform.GetPosition()) * glm::toMat4(glm::quat(glm::radians(transform.GetRotation())));

					Circle circle(0.1f);	//光源圆
					circle.SetColor({ 0.988f, 0.984f, 0.533f, 1 });
					
					switch (light.GetType())
					{
						case Light::Type::Directional:
							glm::vec4 endPos = trans * glm::vec4(0, 0, -10.0f, 1);
							Renderer3D::DrawCircle(trans, circle);									//绘制光源圆
							Renderer3D::DrawLine(position, endPos, { 0.988f, 0.984f, 0.533f, 1 });	//绘制平行光照方向
							break;
						case Light::Type::Point:
							{
								Circle circleRange(range);	//点光源范围圆
								circleRange.SetColor({ 0.988f, 0.984f, 0.533f, 1 });
								circleRange.SetThickness(0.01f);

								//面向相机
								trans = glm::translate(glm::mat4(1.0f), position) * glm::toMat4(m_EditorCamera.GetOrientation());
								
								Renderer3D::DrawCircle(trans, circleRange);	//绘制范围圆
								Renderer3D::DrawCircle(trans, circle);		//绘制光源圆
							}
							break;
						case Light::Type::Spot:
							{
								Renderer3D::DrawCircle(trans, circle);		//绘制光源圆

								glm::vec3 endCirclePos = trans * glm::vec4(0, 0, -range, 1);				//末端圆心位置
								float radius = glm::tan(light.GetSpotOuterAngle() / 2.0f) * range * 2.0f;	//末端圆半径
								Circle spotCircle(radius);	//张角圆
								spotCircle.SetColor({ 0.988f, 0.984f, 0.533f, 1 });
								spotCircle.SetThickness(0.015f);

								glm::vec3 endLinePos[4];	//中间连线末端位置
								endLinePos[0] = trans * glm::vec4(-radius, 0, -range, 1.0f);	//左
								endLinePos[1] = trans * glm::vec4(radius, 0, -range, 1.0f);		//右
								endLinePos[2] = trans * glm::vec4(0, -radius, -range, 1.0f);	//下
								endLinePos[3] = trans * glm::vec4(0, radius, -range, 1.0f);		//上

								trans = glm::translate(glm::mat4(1.0f), endCirclePos)
										* glm::toMat4(glm::quat(glm::radians(transform.GetRotation())));
								
								//绘制中间连线
								for (int i = 0; i < 4; i++) {
									Renderer3D::DrawLine(position, endLinePos[i], { 0.988f, 0.984f, 0.533f, 1 });
								}

								Renderer3D::DrawCircle(trans, spotCircle);	//绘制张角圆
							}
							break;
					}
				}
			}
		}

		//========================Physic==================================
		//绘制CircleCollider2D
		{
			auto view = m_ActiveScene->GetAllObjectsWith<Transform, CircleCollider2D>();
			for (auto object : view) {
				auto [transform, circleCollider] = view.get<Transform, CircleCollider2D>(object);
				Object obj = { object, m_ActiveScene.get() };
				//object启用
				if (obj.GetEnable() && selectedObject == obj) {
					//CircleCollider2D组件启用
					if (circleCollider.GetEnable()) {
						//渲染Circle
						if (circleCollider.GetRenderCircle()) {
							Circle& circle = circleCollider.GetCircle();
							circle.SetColor({ 0.59f, 0.99f, 0.35f, 1.0f });
							circle.SetThickness(0.015f);

							//设置Transform
							glm::vec3 translation = transform.GetPosition() + glm::vec3(circleCollider.GetOffset(), 0.001f);	//位置 + 偏移量：z坐标向摄像机方向偏移
							glm::vec3 scale = glm::max(transform.GetScale().x, transform.GetScale().y) * glm::vec3(circleCollider.GetRadius() * 2.0f);	//缩放值 * 直径

							glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation)
								* glm::scale(glm::mat4(1.0f), scale);

							Renderer3D::DrawCircle(trans, circle);	//绘制Circle碰撞体
						}
					}
				}
			}
		}

		//绘制BoxCollider2D
		{
			auto view = m_ActiveScene->GetAllObjectsWith<Transform, BoxCollider2D>();
			for (auto object : view) {
				Object obj = { object, m_ActiveScene.get() };
				//object启用
				if (obj.GetEnable() && selectedObject == obj) {
					auto [transform, boxCollider] = view.get<Transform, BoxCollider2D>(object);
					//BoxCollider2D组件启用
					if (boxCollider.GetEnable()) {
						//渲染Box
						if (boxCollider.GetRenderBox()) {
							Rectangle& rectangle = boxCollider.GetRectangle();
							rectangle.SetColor({ 0.59f, 0.99f, 0.35f, 1.0f });

							//设置Transform
							glm::vec3 translation = transform.GetPosition() + glm::vec3(boxCollider.GetOffset(), 0.001f);	//位置 + 偏移量：z坐标向摄像机方向偏移
							glm::vec3 scale = transform.GetScale() * glm::vec3(boxCollider.GetSize(), 1.0f);				//缩放值 * 大小

							glm::mat4 trans = glm::translate(glm::mat4(1.0f), translation)
								* glm::rotate(glm::mat4(1.0f), glm::radians(transform.GetRotation().z), { 0.0f, 0.0f, 1.0f })
								* glm::scale(glm::mat4(1.0f), scale);

							Renderer3D::DrawRect(trans, rectangle);	//绘制Rectangle碰撞体
						}
					}
				}
			}
		}

		Renderer3D::EndScene();
	}

	void EditorLayer::NewScene(const std::string& name, bool createDefaultObject)
	{
		//不是编辑状态 停止场景
		if (m_SceneState != SceneState::Edit) {
			OnSceneStop();
		}

		m_EditorScene = std::make_shared<Scene>(name);	//创建新场景

		//创建默认物体
		if (createDefaultObject) {
			m_MainCamera = m_EditorScene->CreateCameraObject("Main Camera", true);	//创建默认Camera对象：主相机
			m_Light = m_EditorScene->CreateLightObject();							//创建默认Light对象
			m_Cube = m_EditorScene->CreateMeshObject("Cube", Mesh::Type::Cube);		//创建默认Cube对象
		}

		m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//重置视口大小
		m_SceneHierarchyPanel.SetScene(m_EditorScene);	//设置Hierarchy的场景

		m_ActiveScene = m_EditorScene;

		//m_EditorScenePath = std::filesystem::path();	//设置当前场景路径
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
		//不是编辑状态 停止场景
		if (m_SceneState != SceneState::Edit) {
			OnSceneStop();
		}

		std::shared_ptr<Scene> newScene = std::make_shared<Scene>();	//创建新场景
		SceneSerializer serializer(newScene);							//场景序列化器

		//反序列化：加载文件场景到新场景
		if (serializer.Deserialize(path.string())) {
			m_EditorScene = newScene;	//设置编辑器场景
			m_EditorScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//重置视口大小
			m_SceneHierarchyPanel.SetScene(m_EditorScene);	//设置Hierarchy的场景

			m_ActiveScene = m_EditorScene;	//设置活动场景
			m_EditorScenePath = path;		//当前场景路径
		}
	}

	void EditorLayer::SaveSceneAs()
	{
		std::string filepath = FileDialogs::SaveFile("Explorer Scene(*.explor)\0*.explor\0");	//保存文件对话框（文件类型名\0 文件类型.explor）

		//路径不为空
		if (!filepath.empty()) {
			std::string sceneName = std::filesystem::path(filepath).stem().string();	//场景名（场景文件名）
			m_ActiveScene->SetName(sceneName);

			SerializeScene(m_ActiveScene, filepath);	//序列化场景

			m_EditorScenePath = filepath;				//当前场景路径
		}
	}

	void EditorLayer::SaveScene()
	{
		//当前场景路径不为空
		if (!m_EditorScenePath.empty()) {
			SerializeScene(m_ActiveScene, m_EditorScenePath);	//序列化场景
		}
		else {
			SaveSceneAs();
		}
	}


	void EditorLayer::SerializeScene(std::shared_ptr<Scene> scene, const std::filesystem::path& path)
	{
		SceneSerializer serializer(scene);		//场景序列化器
		serializer.Serialize(path.string());	//序列化：保存场景
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
		mesh.SetPath(path.string());		//设置Mesh路径

		object.AddComponent<Mesh>(mesh);	//添加Mesh组件
		object.AddComponent<Material>();	//添加Material组件
	}

	void EditorLayer::OnScenePlay()
	{
		m_SceneState = SceneState::Play;	//开始运行

		m_ActiveScene = Scene::Copy(m_EditorScene);	//复制编辑器场景
		m_ActiveScene->OnRuntimeStart();

		m_SceneHierarchyPanel.SetScene(m_ActiveScene);	//设置Hierarchy的场景
	}

	void EditorLayer::OnSceneStop()
	{
		m_SceneState = SceneState::Edit;	//停止运行

		m_ActiveScene->OnRuntimeStop();
		m_ActiveScene = m_EditorScene;		//设置活动场景为编辑器场景

		m_SceneHierarchyPanel.SetScene(m_ActiveScene);	//设置Hierarchy的场景
	}

	void EditorLayer::OnCopyObject()
	{
		if (m_SceneState != SceneState::Edit) return;

		Object object = m_SceneHierarchyPanel.GetSelectedObject();	//被选中物体

		if (object) {
			m_ActiveScene->CopyObject(object);	//复制选中物体
		}
	}

	void EditorLayer::OnDeleteObject()
	{
		if (m_SceneState != SceneState::Edit) return;

		Object object = m_SceneHierarchyPanel.GetSelectedObject();

		if (object) {
			m_ActiveScene->DestroyObject(object);			//删除选中物体
			m_SceneHierarchyPanel.SetSelectedObject({});	//选中物体设置为空
		}
	}
}