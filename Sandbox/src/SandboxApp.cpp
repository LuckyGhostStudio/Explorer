#include <Explorer.h>

#include "imgui/imgui.h"

/// <summary>
/// 示例层
/// </summary>
class ExampleLayer :public Explorer::Layer
{
private:
	std::shared_ptr<Explorer::Shader> m_Shader;				//着色器
	std::shared_ptr<Explorer::VertexArray> m_VertexArray;	//顶点数组

	Explorer::Camera m_Camera;								//相机

	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraMoveSpeed = 0.1f;
	float m_CameraRotationSpeed = 1.0f;
public:
	ExampleLayer() :Layer("Example"), m_Camera(-1.6f, 1.6f, -0.9f, 0.9f), m_CameraPosition(0.0f)
	{ 
		m_VertexArray.reset(new Explorer::VertexArray());		//创建顶点数组对象

		float vertices[] = {
			//------位置------   ---------颜色---------
			-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,	//左下
			 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 1.0f, 1.0f,	//右下
			 0.0f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f,	//上
		};

		std::shared_ptr<Explorer::VertexBuffer> vertexBuffer;								//VBO
		vertexBuffer.reset(new Explorer::VertexBuffer(vertices, sizeof(vertices)));		//创建顶点缓冲

		//顶点缓冲区布局
		Explorer::BufferLayout layout = {
			{Explorer::ShaderDataType::Float3, "a_Position"},	//位置
			{Explorer::ShaderDataType::Float4, "a_Color"}		//颜色
		};

		vertexBuffer->SetLayout(layout);				//设置顶点缓冲区布局
		m_VertexArray->AddVertexBuffer(vertexBuffer);	//添加VBO到VAO

		unsigned int indices[3] = { 0,1,2 };	//顶点索引

		std::shared_ptr<Explorer::IndexBuffer> indexBuffer;												//EBO
		indexBuffer.reset(new Explorer::IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));		//创建索引缓冲
		m_VertexArray->SetIndexBuffer(indexBuffer);	//设置EBO到VAO

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

		m_Shader.reset(new Explorer::Shader(vertexSrc, fragmentSrc));	//创建着色器
	}

	void OnUpdate() override
	{
		if (Explorer::Input::IsKeyPressed(EXP_KEY_LEFT)) {	//左键
			m_CameraPosition.x -= m_CameraMoveSpeed;
		}
		else if (Explorer::Input::IsKeyPressed(EXP_KEY_RIGHT)) {	//右键
			m_CameraPosition.x += m_CameraMoveSpeed;
		}
		if (Explorer::Input::IsKeyPressed(EXP_KEY_UP)) {	//上键
			m_CameraPosition.y += m_CameraMoveSpeed;
		}
		else if (Explorer::Input::IsKeyPressed(EXP_KEY_DOWN)) {	//下键
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

		Explorer::Renderer::BeginScene(m_Camera);				//开始渲染场景
		Explorer::Renderer::Submit(m_Shader, m_VertexArray);	//提交渲染命令
		Explorer::Renderer::EndScene();						//结束渲染场景
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
		PushLayer(new ExampleLayer());	//添加到层栈
	}

	~Sandbox()
	{

	}
};

Explorer::Application* Explorer::CreateApplication()
{
	return new Sandbox();
}