#include "EditorLayer.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

namespace Explorer
{
	EditorLayer::EditorLayer() :Layer("EditorLayer"), m_CameraController(1280.0f / 720.0f) {}

	void EditorLayer::OnAttach()
	{
		m_CheckerboardTexture = std::make_shared<Texture2D>("asserts/textures/Checkerboard.png");	//创建纹理

		FramebufferSpecification fbSpec;	//帧缓冲区规范
		fbSpec.Width = 1280;
		fbSpec.Height = 720;
		m_Framebuffer = std::make_shared < Framebuffer>(fbSpec);	//创建帧缓冲区

		m_ActiveScene = std::make_shared<Scene>();		//创建场景

		m_SquareEntity = m_ActiveScene->CreateEntity();	//创建正方形实体
		m_ActiveScene->Reg().emplace<Transform>(m_SquareEntity);	//添加Transform组件
		m_ActiveScene->Reg().emplace<SpriteRenderer>(m_SquareEntity, glm::vec4(0.0f, 1.0f, 0.0f, 1.0f));	//添加SpriteRenderer组件
	}

	void EditorLayer::OnDetach()
	{

	}

	void EditorLayer::OnUpdate(DeltaTime dt)
	{
		fps = 1.0f / dt;	//帧率

		if (m_ViewportFocused) {	//视口被聚焦
			m_CameraController.OnUpdate(dt);	//更新相机控制器
		}

		//Renderer
		Renderer2D::ResetStats();	//重置统计数据
		m_Framebuffer->Bind();										//绑定帧缓冲区
		RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });	//设置清屏颜色
		RenderCommand::Clear();										//清除

		Renderer2D::BeginScene(m_CameraController.GetCamera());		//开始渲染场景
		m_ActiveScene->OnUpdate(dt);	//更新场景
		Renderer2D::EndScene();			//结束渲染场景

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

			//属性面板
			ImGui::Begin("Settings");

			auto& squarePos = m_ActiveScene->Reg().get<Transform>(m_SquareEntity).m_Position;		//位置
			auto& squareRot = m_ActiveScene->Reg().get<Transform>(m_SquareEntity).m_Rotation;		//旋转
			auto& squareScale = m_ActiveScene->Reg().get<Transform>(m_SquareEntity).m_Scale;		//缩放
			auto& squareColor = m_ActiveScene->Reg().get<SpriteRenderer>(m_SquareEntity).m_Color;	//颜色

			ImGui::SliderFloat3("Position", glm::value_ptr(squarePos), -10.0f, 10.0f);
			ImGui::SliderFloat3("Rotation", glm::value_ptr(squareRot), -360.0f, 360.0f);
			ImGui::SliderFloat3("Scale", glm::value_ptr(squareScale), 0.0f, 10.0f);
			ImGui::ColorEdit4("Color", glm::value_ptr(squareColor));	//颜色编辑UI
			ImGui::SliderFloat2("Texture Tiling Factor", glm::value_ptr(m_TextureTilingFactor), 0.0f, 10.0f);
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
			//视口大小 != Gui面板大小
			if (m_ViewportSize != *((glm::vec2*)&viewportPanelSize) && viewportPanelSize.x > 0 && viewportPanelSize.y > 0) {
				m_Framebuffer->Resize((uint32_t)viewportPanelSize.x, (uint32_t)viewportPanelSize.y);	//重置帧缓冲区大小
				m_ViewportSize = { viewportPanelSize.x, viewportPanelSize.y };			//视口大小
				m_CameraController.OnResize(viewportPanelSize.x, viewportPanelSize.y);	//重置相机大小
			}
			uint32_t textureID = m_Framebuffer->GetColorAttachmentRendererID();	//颜色缓冲区ID
			ImGui::Image((void*)textureID, ImVec2{ m_ViewportSize.x, m_ViewportSize.y }, ImVec2(0, 1), ImVec2(1, 0));
			ImGui::End();
			ImGui::PopStyleVar();
		}
		ImGui::End();
	}

	void EditorLayer::OnEvent(Event& event)
	{
		m_CameraController.OnEvent(event);	//调用相机事件函数
	}
}