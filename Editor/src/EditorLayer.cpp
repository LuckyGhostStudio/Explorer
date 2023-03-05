#include "EditorLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Explorer
{
	EditorLayer::EditorLayer() :Layer("EditorLayer")//, m_CameraController(1280.0f / 720.0f) 
	{
		
	}

	void EditorLayer::OnAttach()
	{
		m_CheckerboardTexture = std::make_shared<Texture2D>("asserts/textures/Checkerboard.png");	//创建纹理

		FramebufferSpecification fbSpec;	//帧缓冲区规范
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = std::make_shared < Framebuffer>(fbSpec);	//创建帧缓冲区

		m_ActiveScene = std::make_shared<Scene>();	//创建场景

		m_SquareObject = m_ActiveScene->CreateEntity("Square");	//创建正方形对象
		m_SquareObject.AddComponent<SpriteRenderer>(glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));	//添加SpriteRenderer组件

		m_CameraObject = m_ActiveScene->CreateEntity("Main Camera");	//创建相机对象
		m_CameraObject.AddComponent<Camera>();					//添加Camera组件

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
			//m_CameraController.OnResize(m_ViewportSize.x, m_ViewportSize.y);				//重置相机大小

			m_ActiveScene->OnViewportResize((uint32_t)m_ViewportSize.x, (uint32_t)m_ViewportSize.y);	//重置视口大小
		}

		if (m_ViewportFocused) {	//视口被聚焦
			//m_CameraController.OnUpdate(dt);	//更新相机控制器
		}

		//Renderer
		Renderer2D::ResetStats();	//重置统计数据
		m_Framebuffer->Bind();										//绑定帧缓冲区
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });	//设置清屏颜色
		RenderCommand::Clear();										//清除

		m_ActiveScene->OnUpdate(dt);	//更新场景

		m_Framebuffer->Unbind();	//解除绑定帧缓冲区
	}

	void EditorLayer::OnImGuiRender()
	{
		static bool dockSpaceOpen = true;	//停靠空间是否开启
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

		ImGui::Begin("DockSpace Demo", &dockSpaceOpen, window_flags);	//停靠空间
		{
			ImGui::PopStyleVar();

			if (opt_fullscreen)
				ImGui::PopStyleVar(2);

			//UI停靠空间
			ImGuiIO& io = ImGui::GetIO();
			if (io.ConfigFlags & ImGuiConfigFlags_DockingEnable)
			{
				ImGuiID dockspace_id = ImGui::GetID("MyDockspace");
				ImGui::DockSpace(dockspace_id, ImVec2(0.0f, 0.0f), opt_flags);
			}

			//菜单条
			if (ImGui::BeginMenuBar())
			{
				//菜单：File
				if (ImGui::BeginMenu("File"))
				{
					if (ImGui::MenuItem("Exit")) {	//菜单项：退出
						Application::GetInstance().Close();	//退出程序
					}
					ImGui::EndMenu();
				}

				ImGui::EndMenuBar();
			}

			auto& squareTransform = m_SquareObject.GetComponent<Transform>();
			auto& squarePos = squareTransform.m_Position;	//位置
			auto& squareRot = squareTransform.m_Rotation;	//旋转
			auto& squareSca = squareTransform.m_Scale;		//缩放
			auto& squareColor = m_SquareObject.GetComponent<SpriteRenderer>().m_Color;	//颜色

			//属性面板
			ImGui::Begin("Inspector");
			//Transform
			ImGui::Separator();
			ImGui::Text("Square");
			ImGui::Separator();
			ImGui::Text("Transform");
			ImGui::DragFloat3("Position", glm::value_ptr(squarePos));
			ImGui::DragFloat3("Rotation", glm::value_ptr(squareRot));
			ImGui::DragFloat3("Scale", glm::value_ptr(squareSca));
			//Sprite Renderer
			ImGui::Text("Sprite Renderer");
			ImGui::ColorEdit4("Color", glm::value_ptr(squareColor));	//颜色编辑UI
			ImGui::DragFloat2("Texture Tiling Factor", glm::value_ptr(m_TextureTilingFactor));
			ImGui::Separator();

			auto& cameraTransform = m_CameraObject.GetComponent<Transform>();
			auto& cameraPos = cameraTransform.m_Position;	//位置
			auto& cameraRot = cameraTransform.m_Rotation;	//旋转
			auto& cameraSca = cameraTransform.m_Scale;		//缩放
			auto& camera = m_CameraObject.GetComponent<Camera>();
			float cameraSize = camera.GetSize();			//尺寸
			float cameraNear = camera.GetNearClip();		//近裁剪平面
			float cameraFar = camera.GetFarClip();			//远裁剪平面

			ImGui::Separator();
			ImGui::Text("Main Camera");
			ImGui::Separator();
			ImGui::Text("Transform");
			ImGui::DragFloat3("Pos", glm::value_ptr(cameraPos));
			ImGui::DragFloat3("Rot", glm::value_ptr(cameraRot));
			ImGui::DragFloat3("Sca", glm::value_ptr(cameraSca));

			ImGui::Text("Camera");
			if (ImGui::DragFloat("Size", &cameraSize)) {
				camera.SetSize(cameraSize);
			}
			if (ImGui::DragFloat("Near", &cameraNear)) {
				camera.SetNearClip(cameraNear);
			}
			if (ImGui::DragFloat("Far", &cameraFar)) {
				camera.SetFarClip(cameraFar);
			}
			ImGui::Separator();

			ImGui::End();

			//批渲染数据统计
			ImGui::Begin("Renderer2D Stats");
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

			m_ViewportFocused = ImGui::IsWindowFocused();	//当前窗口被聚焦
			m_ViewportHovered = ImGui::IsWindowHovered();	//鼠标悬停在当前窗口

			Application::GetInstance().GetImGuiLayer()->BlockEvents(!m_ViewportFocused || !m_ViewportHovered);	//阻止ImGui事件

			ImVec2 viewportPanelSize = ImGui::GetContentRegionAvail();			//Gui面板大小

			m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };		//视口大小

			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();	//颜色缓冲区ID
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
			ImGui::PopStyleVar();
		}
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		//m_CameraController.OnEvent(event);	//调用相机事件函数
	}
}