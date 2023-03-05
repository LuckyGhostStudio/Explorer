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
	m_ActiveScene = std::make_shared<Explorer::Scene>();	//��������

	m_CameraObject = m_ActiveScene->CreateObject("Main Camera");	//�����������
	m_CameraObject.AddComponent<Explorer::Camera>();				//���Camera���
}

void Sandbox3D::OnDetach()
{

}

void Sandbox3D::OnUpdate(Explorer::DeltaTime dt)
{
	//m_CameraController.OnUpdate(dt);	//�������������

	Explorer::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });	//����������ɫ
	Explorer::RenderCommand::Clear();									//���

	Explorer::Renderer3D::BeginScene(m_CameraObject.GetComponent<Explorer::Camera>(), m_CameraObject.GetComponent<Explorer::Transform>());

	auto& transform = Explorer::Transform(m_CubePosition, m_CubeRotation, m_CubeScale);
	Explorer::Renderer3D::DrawMesh(transform);

	Explorer::Renderer3D::EndScene();						//������Ⱦ����
}

void Sandbox3D::OnImGuiRender()
{
	//�������
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Color", glm::value_ptr(m_CubeColor));	//��ɫ�༭UI
	ImGui::DragFloat3("Position", glm::value_ptr(m_CubePosition));
	ImGui::DragFloat3("Rotation", glm::value_ptr(m_CubeRotation));
	ImGui::DragFloat3("Scale", glm::value_ptr(m_CubeScale));
	ImGui::End();
}

void Sandbox3D::OnEvent(Explorer::Event& event)
{
	//m_CameraController.OnEvent(event);	//��������¼�����
}