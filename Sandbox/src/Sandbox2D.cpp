#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Timer.h"



Sandbox2D::Sandbox2D() :Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = std::make_shared<Explorer::Texture2D>("asserts/textures/Checkerboard.png");	//创建纹理
	m_SpriteSheet = std::make_shared<Explorer::Texture2D>("asserts/textures/RPGpack_sheet_2X.png");		//创建纹理

	m_TextureStairs = Explorer::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 6 }, { 128.0f, 128.0f });	//创建楼梯纹理
	m_TextureBarrel = Explorer::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8, 2 }, { 128.0f, 128.0f });	//创建楼梯纹理
	m_TextureTree = Explorer::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128.0f, 128.0f }, { 1, 2 });	//创建树纹理
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
#if 0
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
	}
#endif
	Explorer::Renderer2D::BeginScene(m_CameraController.GetCamera());		//开始渲染场景
	Explorer::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, 0.0f, { 1.0f, 1.0f, 1.0f }, glm::vec4(1.0f), m_TextureStairs);	//绘制楼梯纹理
	Explorer::Renderer2D::DrawQuad({ 1.0f, 0.0f, -0.1f }, 0.0f, { 1.0f, 1.0f, 1.0f }, glm::vec4(1.0f), m_TextureBarrel);	//绘制木桶纹理
	Explorer::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.1f }, 0.0f, { 1.0f, 2.0f, 1.0f }, glm::vec4(1.0f), m_TextureTree);		//绘制树纹理
	Explorer::Renderer2D::EndScene();						//结束渲染场景
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	//属性面板
	ImGui::Text("Inspector:");
	ImGui::ColorEdit4("Color", glm::value_ptr(m_SquareColor));	//颜色编辑UI
	ImGui::SliderFloat3("Position", glm::value_ptr(m_SquarePosition), -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotation", glm::value_ptr(m_SquareRotation), -360.0f, 360.0f);
	ImGui::SliderFloat3("Scale", glm::value_ptr(m_SquareScale), 0.0f, 10.0f);
	ImGui::SliderFloat2("Texture Tiling Factor", glm::value_ptr(m_TextureTilingFactor), 0.0f, 10.0f);

	//性能分析
	ImGui::Text("\nProfiling:");
	for (auto& result : m_ProfileResults) {
		char label[50];
		strcpy(label, "%.3fms  ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);	//显示性能测试结果（测试程序段名 运行时间）
	}
	m_ProfileResults.clear();
	ImGui::Text("FPS: %.3f", fps);	//帧率

	//批渲染数据统计
	ImGui::Text("\nRenderer2D Stats:");
	auto stats = Explorer::Renderer2D::GetStats();
	ImGui::Text("Draw Calls: %d", stats.DrawCalls);
	ImGui::Text("Quad: %d", stats.QuadCount);
	ImGui::Text("Vertices: %d", stats.GetTotalVertexCount());
	ImGui::Text("Indices: %d", stats.GetTotalIndexCount());

	ImGui::End();
}

void Sandbox2D::OnEvent(Explorer::Event& event)
{
	m_CameraController.OnEvent(event);	//调用相机事件函数
}