#include <Explorer.h>

#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

/// <summary>
/// 示例层
/// </summary>
class ExampleLayer :public Explorer::Layer
{
private:
	std::shared_ptr<Explorer::Shader> m_Shader;				//着色器
	std::shared_ptr<Explorer::VertexArray> m_VertexArray;	//顶点数组
	std::shared_ptr<Explorer::Shader> m_FlatColorShader, m_TextureShader;
	std::shared_ptr<Explorer::VertexArray> m_SquareVA;

	std::shared_ptr<Explorer::Texture2D> m_Texture, m_ChernoLogoTexture;

	Explorer::Camera m_Camera;								//相机

	glm::vec3 m_CameraPosition;
	float m_CameraRotation = 0.0f;
	float m_CameraMoveSpeed = 1.0f;
	float m_CameraRotationSpeed = 90.0f;

	glm::vec3 m_SquareColor = { 0.2f,0.3f,0.8f };
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

		m_Shader.reset(new Explorer::Shader(vertexSrc, fragmentSrc));	//创建着色器

		//Square
		m_SquareVA.reset(new Explorer::VertexArray());		//创建顶点数组对象

		float squareVertices[] = {
			//------位置------  --纹理坐标--
			-0.5f, -0.5f, 0.0f,	0.0f, 0.0f,	//左下0
			 0.5f, -0.5f, 0.0f,	1.0f, 0.0f,	//右下1
			-0.5f,  0.5f, 0.0f,	0.0f, 1.0f,	//左上2
			 0.5f,  0.5f, 0.0f,	1.0f, 1.0f	//右上3
		};

		std::shared_ptr<Explorer::VertexBuffer> squareVB;									//VBO
		squareVB.reset(new Explorer::VertexBuffer(squareVertices, sizeof(squareVertices)));		//创建顶点缓冲

		squareVB->SetLayout({
			{Explorer::ShaderDataType::Float3, "a_Position"},	//位置
			{Explorer::ShaderDataType::Float2, "a_TexCoord"},	//纹理坐标
			});
		m_SquareVA->AddVertexBuffer(squareVB);	//添加VBO到VAO

		unsigned int squareIndices[] = { 0, 1, 2, 2, 1, 3 };		//顶点索引

		std::shared_ptr<Explorer::IndexBuffer> squareIB;														//EBO
		squareIB.reset(new Explorer::IndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));		//创建索引缓冲
		m_SquareVA->SetIndexBuffer(squareIB);														//设置EBO到VAO

		std::string squareVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;					

			void main()
			{
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string squareFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			uniform vec3 u_Color;
			
			void main()
			{
				color = vec4(u_Color, 1.0);
			}
		)";

		m_FlatColorShader.reset(new Explorer::Shader(squareVertexSrc, squareFragmentSrc));	//创建着色器

		std::string textureVertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec2 a_TexCoord;

			uniform mat4 u_ViewProjectionMatrix;
			uniform mat4 u_Transform;

			out vec2 v_TexCoord;					

			void main()
			{
				v_TexCoord = a_TexCoord;
				gl_Position = u_ViewProjectionMatrix * u_Transform * vec4(a_Position, 1.0);
			}
		)";

		std::string textureFragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec2 v_TexCoord;

			uniform sampler2D u_Texture;
			
			void main()
			{
				color = texture(u_Texture, v_TexCoord);
			}
		)";

		m_TextureShader.reset(new Explorer::Shader(textureVertexSrc, textureFragmentSrc));	//创建着色器

		m_Texture.reset(new Explorer::Texture2D("asserts/textures/Checkerboard.png"));		//创建纹理
		m_ChernoLogoTexture.reset(new Explorer::Texture2D("asserts/textures/ChernoLogo.png"));		//创建纹理

		m_TextureShader->Bind();
		m_TextureShader->UploadUniformInt("u_Texture", 0);
	}

	void OnUpdate(Explorer::DeltaTime dt) override
	{
		//EXP_TRACE("Delta Time: {0}s ({1}ms)", dt.GetSeconds(), dt.GetMilliseconds());

		//相机移动
		if (Explorer::Input::IsKeyPressed(EXP_KEY_LEFT)) {	//左键
			m_CameraPosition.x -= m_CameraMoveSpeed * dt;
		}
		else if (Explorer::Input::IsKeyPressed(EXP_KEY_RIGHT)) {	//右键
			m_CameraPosition.x += m_CameraMoveSpeed * dt;
		}
		if (Explorer::Input::IsKeyPressed(EXP_KEY_UP)) {	//上键
			m_CameraPosition.y += m_CameraMoveSpeed * dt;
		}
		else if (Explorer::Input::IsKeyPressed(EXP_KEY_DOWN)) {	//下键
			m_CameraPosition.y -= m_CameraMoveSpeed * dt;
		}
		//相机旋转
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

		Explorer::Renderer::BeginScene(m_Camera);				//开始渲染场景

		static glm::mat4 scale = glm::scale(glm::mat4(1.0f), glm::vec3(0.1f));

		m_FlatColorShader->Bind();
		m_FlatColorShader->UploadUniformFloat3("u_Color", m_SquareColor);

		for (int y = 0; y < 20; y++) {
			for (int x = 0; x < 20; x++) {
				glm::vec3 pos(x * 0.11f, y * 0.11f, 0.0f);
				glm::mat4 transform = glm::translate(glm::mat4(1.0f), pos) * scale;		//三角形的变换矩阵
				Explorer::Renderer::Submit(m_FlatColorShader, m_SquareVA, transform);	//提交渲染指令
			}
		}

		m_Texture->Bind();
		Explorer::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));
		m_ChernoLogoTexture->Bind();
		Explorer::Renderer::Submit(m_TextureShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));

		//Triangle
		//Explorer::Renderer::Submit(m_Shader, m_VertexArray);	//提交渲染指令

		Explorer::Renderer::EndScene();						//结束渲染场景
	}

	void OnImGuiRender() override
	{
		ImGui::Begin("Settings");
		ImGui::ColorEdit3("Triangle Color", glm::value_ptr(m_SquareColor));
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