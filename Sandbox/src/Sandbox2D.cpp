#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() :Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = std::make_shared<Explorer::Texture2D>("asserts/textures/Checkerboard.png");	//��������
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Explorer::DeltaTime dt)
{
	m_CameraController.OnUpdate(dt);	//�������������

	//Renderer
	Explorer::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });	//����������ɫ
	Explorer::RenderCommand::Clear();									//���

	Explorer::Renderer2D::BeginScene(m_CameraController.GetCamera());		//��ʼ��Ⱦ����

	Explorer::Renderer2D::DrawQuad(m_SquarePosition, m_SquareRotation.z, m_SquareScale, m_SquareColor, m_CheckerboardTexture);	//�����ı���
	Explorer::Renderer2D::DrawQuad({ -1.0f, 0.0f }, 0.0f, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });	//�����ı���
	Explorer::Renderer2D::DrawQuad({ 0.5f, -0.5f }, 0.0f,{ 0.5f, 0.75f }, { 0.2f, 0.8f, 0.3f, 1.0f });	//�����ı���

	Explorer::Renderer2D::EndScene();						//������Ⱦ����
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Color", glm::value_ptr(m_SquareColor));	//��ɫ�༭UI

	ImGui::SliderFloat3("Position", glm::value_ptr(m_SquarePosition), -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotation", glm::value_ptr(m_SquareRotation), -360.0f, 360.0f);
	ImGui::SliderFloat3("Scale", glm::value_ptr(m_SquareScale), 0.0f, 10.0f);

	ImGui::End();
}

void Sandbox2D::OnEvent(Explorer::Event& event)
{
	m_CameraController.OnEvent(event);	//��������¼�����
}