#include <Explorer.h>

#include "imgui/imgui.h"

/// <summary>
/// ʾ����
/// </summary>
class ExampleLayer :public Explorer::Layer
{
private:
	std::shared_ptr<Explorer::Shader> m_Shader;				//��ɫ��
	std::shared_ptr<Explorer::VertexArray> m_VertexArray;	//��������

	Explorer::Camera m_Camera;								//���

	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraMoveSpeed = 0.1f;
	float m_CameraRotationSpeed = 1.0f;
public:
	ExampleLayer() :Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
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

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;

			out vec3 v_Position;			
			out vec4 v_Color;			

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				color = v_Color;
			}
		)";

		m_Shader.reset(new Explorer::Shader(vertexSrc, fragmentSrc));	//������ɫ��
	}

	void OnUpdate() override
	{
		if (Explorer::Input::IsKeyPressed(EXP_KEY_LEFT)) {	//���
			m_CameraPosition.x -= m_CameraMoveSpeed;
		}
		else if (Explorer::Input::IsKeyPressed(EXP_KEY_RIGHT)) {	//�Ҽ�
			m_CameraPosition.x += m_CameraMoveSpeed;
		}
		if (Explorer::Input::IsKeyPressed(EXP_KEY_UP)) {	//�ϼ�
			m_CameraPosition.y += m_CameraMoveSpeed;
		}
		else if (Explorer::Input::IsKeyPressed(EXP_KEY_DOWN)) {	//�¼�
			m_CameraPosition.y -= m_CameraMoveSpeed;
		}

		if (Explorer::Input::IsKeyPressed(EXP_KEY_A)) {
			m_CameraRotation += m_CameraRotationSpeed;
		}
		if (Explorer::Input::IsKeyPressed(EXP_KEY_D)) {
			m_CameraRotation -= m_CameraRotationSpeed;
		}

		Explorer::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Explorer::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Explorer::Renderer::BeginScene(m_Camera);				//��ʼ��Ⱦ����
		Explorer::Renderer::Submit(m_Shader, m_VertexArray);	//�ύ��Ⱦ����
		Explorer::Renderer::EndScene();						//������Ⱦ����
	}

	void OnImGuiRender() override
	{
		
	}

	void OnEvent(Explorer::Event& event) override
	{
		
	}
};

class Sandbox :public Explorer::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());	//��ӵ���ջ
	}

	~Sandbox()
	{

	}
};

Explorer::Application* Explorer::CreateApplication()
{
	return new Sandbox();
}