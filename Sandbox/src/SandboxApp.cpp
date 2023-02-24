#include <Explorer.h>
#include <Explorer/Core/EntryPoint.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Explorer/Renderer/Shader.h"
#include "Sandbox2D.h"

/// <summary>
/// ʾ����
/// </summary>
class ExampleLayer :public Explorer::Layer
{
private:
	Explorer::ShaderLibrary m_ShaderLibrary;				//��ɫ����
	std::shared_ptr<Explorer::Shader> m_Shader;				//��ɫ��
	std::shared_ptr<Explorer::VertexArray> m_VertexArray;	//��������
	std::shared_ptr<Explorer::Shader> m_FlatColorShader;
	std::shared_ptr<Explorer::VertexArray> m_SquareVA;

	std::shared_ptr<Explorer::Texture2D> m_Texture, m_ChernoLogoTexture;

	Explorer::CameraController m_CameraController;			//���������

	glm::vec3 m_SquareColor = { 0.2f,0.3f,0.8f };
public:
	ExampleLayer() :Layer("Example"), m_CameraController(1280.0f / 720.0f)
	{ 
		m_VertexArray.reset(new Explorer::VertexArray());		//���������������

		float vertices[] = {
			//------λ��------   ---------��ɫ---------
			-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,	//����
			 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 1.0f, 1.0f,	//����
			 0.0f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f,	//��
		};

		std::shared_ptr<Explorer::VertexBuffer> vertexBuffer;								//VBO
		vertexBuffer.reset(new Explorer::VertexBuffer(vertices, sizeof(vertices)));		//�������㻺��

		//���㻺��������
		Explorer::BufferLayout layout = {
			{Explorer::ShaderDataType::Float3, "a_Position"},	//λ��
			{Explorer::ShaderDataType::Float4, "a_Color"}		//��ɫ
		};

		vertexBuffer->SetLayout(layout);				//���ö��㻺��������
		m_VertexArray->AddVertexBuffer(vertexBuffer);	//���VBO��VAO

		unsigned int indices[3] = { 0,1,2 };	//��������

		std::shared_ptr<Explorer::IndexBuffer> indexBuffer;												//EBO
		indexBuffer.reset(new Explorer::IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));		//������������
		m_VertexArray->SetIndexBuffer(indexBuffer);	//����EBO��VAO

		m_Shader.reset(new Explorer::Shader("asserts/shaders/TriangleShader"));	//������ɫ��

		//Square
		m_SquareVA.reset(new Explorer::VertexArray());		//���������������

		float squareVertices[] = {
			//------λ��------  --��������--
			-0.5f, -0.5f, 0.0f,	0.0f, 0.0f,	//����0
			 0.5f, -0.5f, 0.0f,	1.0f, 0.0f,	//����1
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f,	//����2
			 0.5f,  0.5f, 0.0f,	1.0f, 1.0f	//����3
		};

		std::shared_ptr<Explorer::VertexBuffer> squareVB;									//VBO
		squareVB.reset(new Explorer::VertexBuffer(squareVertices, sizeof(squareVertices)));		//�������㻺��

		squareVB->SetLayout({
			{Explorer::ShaderDataType::Float3, "a_Position"},	//λ��
			{Explorer::ShaderDataType::Float2, "a_TexCoord"},	//��������
			});
		m_SquareVA->AddVertexBuffer(squareVB);	//���VBO��VAO

		unsigned int squareIndices[] = { 0, 1, 2, 2, 1, 3 };		//��������

		std::shared_ptr<Explorer::IndexBuffer> squareIB;														//EBO
		squareIB.reset(new Explorer::IndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));		//������������
		m_SquareVA->SetIndexBuffer(squareIB);														//����EBO��VAO

		m_FlatColorShader.reset(new Explorer::Shader("asserts/shaders/FlatColorShader"));			//������ɫ��

		auto textureShader = m_ShaderLibrary.Load("asserts/shaders/TextureShader");				//���ش�����ɫ��

		m_Texture.reset(new Explorer::Texture2D("asserts/textures/Checkerboard.png"));		//��������
		m_ChernoLogoTexture.reset(new Explorer::Texture2D("asserts/textures/ChernoLogo.png"));		//��������

		textureShader->Bind();
		textureShader->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Explorer::DeltaTime dt) override
	{
		m_CameraController.OnUpdate(dt);	//�������������

		Explorer::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Explorer::RenderCommand::Clear();

		Explorer::Renderer::BeginScene(m_CameraController.GetCamera());				//��ʼ��Ⱦ����

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		m_FlatColorShader->Bind();
		m_FlatColorShader->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;		//�����εı任����
				Explorer::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);	//�ύ��Ⱦָ��
			}
		}

		auto textureShader = m_ShaderLibrary.Get("TextureShader");

		m_Texture->Bind();
		Explorer::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_ChernoLogoTexture->Bind();
		Explorer::Renderer::Submit(textureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//Triangle
		//Explorer::Renderer::Submit(m_Shader, m_VertexArray);	//�ύ��Ⱦָ��

		Explorer::Renderer::EndScene();						//������Ⱦ����
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_SquareColor));
		ImGui::End();
	}

	void OnEvent(Explorer::Event& event) override
	{
		m_CameraController.OnEvent(event);	//��������¼����� 
	}
};

class Sandbox :public Explorer::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());	//���ʾ������ջ
		PushLayer(new Sandbox2D());		//���2D��Ⱦ���Բ�
	}

	~Sandbox()
	{

	}
};

Explorer::Application* Explorer::CreateApplication()
{
	return new Sandbox();
}