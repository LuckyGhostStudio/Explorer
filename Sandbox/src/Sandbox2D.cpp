#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() :Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Explorer::DeltaTime dt)
{
	m_CameraController.OnUpdate(dt);	//更新相机控制器

	//Renderer
	Explorer::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });	//设置清屏颜色
	Explorer::RenderCommand::Clear();									//清除

	Explorer::Renderer2D::BeginScene(m_CameraController.GetCamera());		//开始渲染场景

	Explorer::Renderer2D::DrawQuad({ 0.0f, 0.0f }, { 1.0f, 1.0f }, { 0.8f, 0.2f, 0.3f, 1.0f });	//绘制四边形

	Explorer::Renderer2D::EndScene();						//结束渲染场景

	//m_FlatColorShader->Bind();
	//m_FlatColorShader->UploadUniformFloat4("u_Color", m_SquareColor);	//上传uniform变量
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));	//颜色编辑UI
	ImGui::End();
}

void Sandbox2D::OnEvent(Explorer::Event& event)
{
	m_CameraController.OnEvent(event);	//调用相机事件函数
}