#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() :Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	m_SquareVA = std::make_shared<Explorer::VertexArray>();		//���������������

	float squareVertices[] = {
		//------λ��------  
		-0.5f, -0.5f, 0.0f,	//����0
		 0.5f, -0.5f, 0.0f,	//����1
		-0.5f,  0.5f, 0.0f,	//����2
		 0.5f,  0.5f, 0.0f  //����3
	};

	std::shared_ptr<Explorer::VertexBuffer> squareVB;									//VBO
	squareVB.reset(new Explorer::VertexBuffer(squareVertices, sizeof(squareVertices)));	//�������㻺��

	//���ö��㻺��������
	squareVB->SetLayout({
		{Explorer::ShaderDataType::Float3, "a_Position"},	//λ��
		});
	m_SquareVA->AddVertexBuffer(squareVB);	//���VBO��VAO

	unsigned int squareIndices[] = { 0, 1, 2, 2, 1, 3 };		//��������

	std::shared_ptr<Explorer::IndexBuffer> squareIB;														//EBO
	squareIB.reset(new Explorer::IndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));		//������������
	m_SquareVA->SetIndexBuffer(squareIB);																	//����EBO��VAO

	m_FlatColorShader = std::make_shared<Explorer::Shader>("asserts/shaders/FlatColorShader");				//������ɫ��
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

	Explorer::Renderer::BeginScene(m_CameraController.GetCamera());		//��ʼ��Ⱦ����

	m_FlatColorShader->Bind();
	m_FlatColorShader->UploadUniformFloat4("u_Color", m_SquareColor);	//�ϴ�uniform����

	Explorer::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));	//�ύ��Ⱦָ��

	Explorer::Renderer::EndScene();						//������Ⱦ����
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));	//��ɫ�༭UI
	ImGui::End();
}

void Sandbox2D::OnEvent(Explorer::Event& event)
{
	m_CameraController.OnEvent(event);	//��������¼�����
}