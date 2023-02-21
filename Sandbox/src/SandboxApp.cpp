#include <Explorer.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

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
	float m_CameraMoveSpeed = 1.0f;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_TriangleColor = { 0.2f,0.3f,0.8f };
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
			uniform mat4 u_Transform;

			out vec3 v_Position;						

			void main()
			{
				v_Position = a_Position;
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;

			uniform vec3 u_Color;
			
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_Shader.reset(new Explorer::Shader(vertexSrc, fragmentSrc));	//������ɫ��
	}

	void OnUpdate(Explorer::DeltaTime dt) override
	{
		//EXP_TRACE("Delta Time: {0}s ({1}ms)", dt.GetSeconds(), dt.GetMilliseconds());

		//����ƶ�
		if (Explorer::Input::IsKeyPressed(EXP_KEY_LEFT)) {	//���
			m_CameraPosition.x -= m_CameraMoveSpeed * dt;
		}
		else if (Explorer::Input::IsKeyPressed(EXP_KEY_RIGHT)) {	//�Ҽ�
			m_CameraPosition.x += m_CameraMoveSpeed * dt;
		}
		if (Explorer::Input::IsKeyPressed(EXP_KEY_UP)) {	//�ϼ�
			m_CameraPosition.y += m_CameraMoveSpeed * dt;
		}
		else if (Explorer::Input::IsKeyPressed(EXP_KEY_DOWN)) {	//�¼�
			m_CameraPosition.y -= m_CameraMoveSpeed * dt;
		}
		//�����ת
		if (Explorer::Input::IsKeyPressed(EXP_KEY_A)) {
			m_CameraRotation += m_CameraRotationSpeed * dt;
		}
		if (Explorer::Input::IsKeyPressed(EXP_KEY_D)) {
			m_CameraRotation -= m_CameraRotationSpeed * dt;
		}

		Explorer::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
		Explorer::RenderCommand::Clear();

		m_Camera.SetPosition(m_CameraPosition);
		m_Camera.SetRotation(m_CameraRotation);

		Explorer::Renderer::BeginScene(m_Camera);				//��ʼ��Ⱦ����

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		m_Shader->Bind();
		m_Shader->UploadUniformFloat3("u_Color", m_TriangleColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;	//�����εı任����
				Explorer::Renderer::Submit(m_Shader, m_VertexArray, transform);		//�ύ��Ⱦָ��
			}
		}

		Explorer::Renderer::EndScene();						//������Ⱦ����
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_TriangleColor));
		ImGui::End();
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