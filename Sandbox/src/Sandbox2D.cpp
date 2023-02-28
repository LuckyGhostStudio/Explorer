#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Timer.h"



Sandbox2D::Sandbox2D() :Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = std::make_shared<Explorer::Texture2D>("asserts/textures/Checkerboard.png");	//创建纹理
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Explorer::DeltaTime dt)
{
	PROFILE_SCOPE("Sandbox2D::OnUpdate");

	fps = 1.0f / dt;

	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(dt);	//更新相机控制器
	}

	//Renderer
	Explorer::Renderer2D::ResetStats();	//重置统计数据
	{
		PROFILE_SCOPE("Renderer Preparation");
		Explorer::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });	//设置清屏颜色
		Explorer::RenderCommand::Clear();									//清除
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		Explorer::Renderer2D::BeginScene(m_CameraController.GetCamera());		//开始渲染场景

		Explorer::Renderer2D::DrawQuad(m_SquarePosition, m_SquareRotation.z, m_SquareScale, m_SquareColor, m_CheckerboardTexture, m_TextureTilingFactor);	//绘制四边形
		Explorer::Renderer2D::DrawQuad({ -3.0f, 1.0f, 1.0f }, 0.0f, { 0.8f, 0.8f, 1.0f }, { 0.8f, 0.2f, 0.5f, 1.0f }, m_CheckerboardTexture, { 0.5f, 0.5f });	//绘制四边形
		Explorer::Renderer2D::DrawQuad({ 1.0f, 0.0f, 1.0f }, 0.0f, { 0.8f, 0.8f, 1.0f }, { 0.8f, 0.2f, 0.3f, 0.8f });	//绘制四边形
		Explorer::Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.0f }, 0.0f, { 0.5f, 0.75f, 0.0f }, { 0.2f, 0.8f, 0.3f, 1.0f });	//绘制四边形

		Explorer::Renderer2D::EndScene();						//结束渲染场景

		Explorer::Renderer2D::BeginScene(m_CameraController.GetCamera());		//开始渲染场景

		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.6f };
				Explorer::Renderer2D::DrawQuad({ x, y }, 0.0f, { 0.45f, 0.45f }, color);
			}
		}

		Explorer::Renderer2D::EndScene();						//结束渲染场景
	}					//结束渲染场景
}

void Sandbox2D::OnImGuiRender()
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
					Explorer::Application::GetInstance().Close();	//退出程序
				}
				ImGui::EndMenu();
			}

			ImGui::EndMenuBar();
		}

		//属性面板
		ImGui::Begin("Settings");
		ImGui::ColorEdit4("Color", glm::value_ptr(m_SquareColor));	//颜色编辑UI
		ImGui::SliderFloat3("Position", glm::value_ptr(m_SquarePosition), -10.0f, 10.0f);
		ImGui::SliderFloat3("Rotation", glm::value_ptr(m_SquareRotation), -360.0f, 360.0f);
		ImGui::SliderFloat3("Scale", glm::value_ptr(m_SquareScale), 0.0f, 10.0f);
		ImGui::SliderFloat2("Texture Tiling Factor", glm::value_ptr(m_TextureTilingFactor), 0.0f, 10.0f);
		ImGui::End();

		//性能分析
		ImGui::Begin("Profiling");
		for (auto& result : m_ProfileResults) {
			char label[50];
			strcpy(label, "%.3fms  ");
			strcat(label, result.Name);
			ImGui::Text(label, result.Time);	//显示性能测试结果（测试程序段名 运行时间）
		}
		m_ProfileResults.clear();
		ImGui::Text("FPS: %.3f", fps);	//帧率
		ImGui::End();

		//批渲染数据统计
		ImGui::Begin("Renderer2D Stats");
		auto stats = Explorer::Renderer2D::GetStats();
		ImGui::Text("Draw Calls: %d", stats.DrawCalls);
		ImGui::Text("Quad: %d", stats.QuadCount);
		ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
		ImGui::Text("Indices: %d", stats.GetTotalIndexCount());
		ImGui::End();
	}
	ImGui::End();
}

void Sandbox2D::OnEvent(Explorer::Event& event)
{
	m_CameraController.OnEvent(event);	//调用相机事件函数
}