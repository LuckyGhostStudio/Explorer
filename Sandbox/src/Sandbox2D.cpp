#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Timer.h"



Sandbox2D::Sandbox2D() :Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = std::make_shared<Explorer::Texture2D>("asserts/textures/Checkerboard.png");	//��������
	m_SpriteSheet = std::make_shared<Explorer::Texture2D>("asserts/textures/RPGpack_sheet_2X.png");		//��������

	m_TextureStairs = Explorer::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 7, 6 }, { 128.0f, 128.0f });	//����¥������
	m_TextureBarrel = Explorer::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 8, 2 }, { 128.0f, 128.0f });	//����¥������
	m_TextureTree = Explorer::SubTexture2D::CreateFromCoords(m_SpriteSheet, { 2, 1 }, { 128.0f, 128.0f }, { 1, 2 });	//����������
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
		m_CameraController.OnUpdate(dt);	//�������������
	}

	//Renderer
	Explorer::Renderer2D::ResetStats();	//����ͳ������
	{
		PROFILE_SCOPE("Renderer Preparation");
		Explorer::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });	//����������ɫ
		Explorer::RenderCommand::Clear();									//���
	}
#if 0
	{
		PROFILE_SCOPE("Renderer Draw");
		Explorer::Renderer2D::BeginScene(m_CameraController.GetCamera());		//��ʼ��Ⱦ����

		Explorer::Renderer2D::DrawQuad(m_SquarePosition, m_SquareRotation.z, m_SquareScale, m_SquareColor, m_CheckerboardTexture, m_TextureTilingFactor);	//�����ı���
		Explorer::Renderer2D::DrawQuad({ -3.0f, 1.0f, 1.0f }, 0.0f, { 0.8f, 0.8f, 1.0f }, { 0.8f, 0.2f, 0.5f, 1.0f }, m_CheckerboardTexture, { 0.5f, 0.5f });	//�����ı���
		Explorer::Renderer2D::DrawQuad({ 1.0f, 0.0f, 1.0f }, 0.0f, { 0.8f, 0.8f, 1.0f }, { 0.8f, 0.2f, 0.3f, 0.8f });	//�����ı���
		Explorer::Renderer2D::DrawQuad({ 0.5f, -0.5f, 0.0f }, 0.0f, { 0.5f, 0.75f, 0.0f }, { 0.2f, 0.8f, 0.3f, 1.0f });	//�����ı���

		Explorer::Renderer2D::EndScene();						//������Ⱦ����

		Explorer::Renderer2D::BeginScene(m_CameraController.GetCamera());		//��ʼ��Ⱦ����

		for (float y = -5.0f; y < 5.0f; y += 0.5f) {
			for (float x = -5.0f; x < 5.0f; x += 0.5f) {
				glm::vec4 color = { (x + 5.0f) / 10.0f, 0.4f, (y + 5.0f) / 10.0f, 0.6f };
				Explorer::Renderer2D::DrawQuad({ x, y }, 0.0f, { 0.45f, 0.45f }, color);
			}
		}

		Explorer::Renderer2D::EndScene();						//������Ⱦ����
	}
#endif
	Explorer::Renderer2D::BeginScene(m_CameraController.GetCamera());		//��ʼ��Ⱦ����
	Explorer::Renderer2D::DrawQuad({ 0.0f, 0.0f, -0.1f }, 0.0f, { 1.0f, 1.0f, 1.0f }, glm::vec4(1.0f), m_TextureStairs);	//����¥������
	Explorer::Renderer2D::DrawQuad({ 1.0f, 0.0f, -0.1f }, 0.0f, { 1.0f, 1.0f, 1.0f }, glm::vec4(1.0f), m_TextureBarrel);	//����ľͰ����
	Explorer::Renderer2D::DrawQuad({ -1.0f, 0.0f, -0.1f }, 0.0f, { 1.0f, 2.0f, 1.0f }, glm::vec4(1.0f), m_TextureTree);		//����������
	Explorer::Renderer2D::EndScene();						//������Ⱦ����
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");

	//�������
	ImGui::Text("Inspector:");
	ImGui::ColorEdit4("Color", glm::value_ptr(m_SquareColor));	//��ɫ�༭UI
	ImGui::SliderFloat3("Position", glm::value_ptr(m_SquarePosition), -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotation", glm::value_ptr(m_SquareRotation), -360.0f, 360.0f);
	ImGui::SliderFloat3("Scale", glm::value_ptr(m_SquareScale), 0.0f, 10.0f);
	ImGui::SliderFloat2("Texture Tiling Factor", glm::value_ptr(m_TextureTilingFactor), 0.0f, 10.0f);

	//���ܷ���
	ImGui::Text("\nProfiling:");
	for (auto& result : m_ProfileResults) {
		char label[50];
		strcpy(label, "%.3fms  ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);	//��ʾ���ܲ��Խ�������Գ������ ����ʱ�䣩
	}
	m_ProfileResults.clear();
	ImGui::Text("FPS: %.3f", fps);	//֡��

	//����Ⱦ����ͳ��
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
	m_CameraController.OnEvent(event);	//��������¼�����
}