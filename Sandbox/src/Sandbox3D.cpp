#include "Sandbox3D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox3D::Sandbox3D() :Layer("Sandbox3D")//, m_CameraController(45.0f, 1280.0f / 720.0f, { 0.1f, 100.0f })
{
	//m_CameraController.GetCamera().SetPosition({ -1.0f, 0.5f, 3.0f });
	//m_CameraController.GetCamera().SetRotation({ -10.0f, -20.0f, 0.0f });
}

void Sandbox3D::OnAttach()
{
	m_ActiveScene = std::make_shared<Explorer::Scene>();	//创建场景

	m_CameraObject = m_ActiveScene->CreateObject("Main Camera");	//创建相机对象
	m_CameraObject.AddComponent<Explorer::Camera>();				//添加Camera组件
}

void Sandbox3D::OnDetach()
{

}

void Sandbox3D::OnUpdate(Explorer::DeltaTime dt)
{
	//m_CameraController.OnUpdate(dt);	//更新相机控制器

	Explorer::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });	//设置清屏颜色
	Explorer::RenderCommand::Clear();									//清除

	Explorer::Renderer3D::BeginScene(m_CameraObject.GetComponent<Explorer::Camera>(), m_CameraObject.GetComponent<Explorer::Transform>());

	auto& transform = Explorer::Transform(m_CubePosition, m_CubeRotation, m_CubeScale);
	Explorer::Renderer3D::DrawMesh(transform);

	Explorer::Renderer3D::EndScene();						//结束渲染场景
}

void Sandbox3D::OnImGuiRender()
{
	//属性面板
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Color", glm::value_ptr(m_CubeColor));	//颜色编辑UI
	ImGui::DragFloat3("Position", glm::value_ptr(m_CubePosition));
	ImGui::DragFloat3("Rotation", glm::value_ptr(m_CubeRotation));
	ImGui::DragFloat3("Scale", glm::value_ptr(m_CubeScale));
	ImGui::End();
}

void Sandbox3D::OnEvent(Explorer::Event& event)
{
	//m_CameraController.OnEvent(event);	//调用相机事件函数
}