#include "exppch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Explorer
{
	Renderer3D::RendererType Renderer3D::m_Type = RendererType::Rasterization;

	/// <summary>
	/// 渲染器数据
	/// </summary>
	struct RendererData
	{
		std::shared_ptr<VertexArray> m_VertexArray;		//四边形顶点数组
		std::shared_ptr<VertexBuffer> m_VertexBuffer;	//四边形顶点缓冲区
		std::shared_ptr<Shader>	m_Shader;				//纹理着色器
	};

	static RendererData s_Data;

	void Renderer3D::Init()
	{
		s_Data.m_VertexArray = std::make_shared<VertexArray>();	//创建VAO

		//正方体顶点
		float cubeVertices[] =
		{
            //-----位置坐标-----  ---------颜色---------  ------法向量------
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f, -1.0f,  1.0f,	// A 0
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f, -1.0f, -1.0f,	// B 1
			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,  1.0f, -1.0f,	// C 2
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,  1.0f,  1.0f,	// D 3
			-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f,  1.0f,	// E 4
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f,	// F 5
			-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  1.0f, -1.0f,	// G 6
			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  1.0f,  1.0f,	// H 7
		};

		s_Data.m_VertexBuffer = std::make_shared<VertexBuffer>(cubeVertices, 8 * 10 * sizeof(float));	//创建VBO

		//设置顶点缓冲区布局
		s_Data.m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//位置
			{ShaderDataType::Float4, "a_Color"},	//颜色
			{ShaderDataType::Float3, "a_Normal"},	//法线
		});
		s_Data.m_VertexArray->AddVertexBuffer(s_Data.m_VertexBuffer);	//添加VBO到VAO

		//顶点索引
		uint32_t cubeIndices[] =
		{
			0, 1, 2,	// A B C
			2, 3, 0,	// C D A
			6, 5, 4,	// G F E
			4, 7, 6,	// G H E
			7, 2, 6,	// H C G
			2, 7, 3,	// C D H
			4, 5, 1,	// E F B
			1, 0, 4,	// B A E
			7, 4, 0,	// H E A
			0, 3, 7,	// A D H
			6, 1, 5,	// G B F
			1, 6, 2,	// B G C
		};

		std::shared_ptr<IndexBuffer> cubeEBO = std::make_shared<IndexBuffer>(cubeIndices, 36);	//创建EBO
		s_Data.m_VertexArray->SetIndexBuffer(cubeEBO);											//设置EBO到VAO

		s_Data.m_Shader = std::make_shared<Shader>("asserts/shaders/DefaultShader");	//创建默认着色器
		s_Data.m_Shader->Bind();														//绑定着色器
	}

	void Renderer3D::Shutdown()
	{

	}

	void Renderer3D::BeginScene(const Camera& camera, const Transform& transform)
	{
		glm::mat4 viewProject = camera.GetProjection() * glm::inverse(transform.m_Transform);	//vp = p * v

		s_Data.m_Shader->Bind();			//绑定着色器
		s_Data.m_Shader->SetMat4("u_ViewProjectionMatrix", viewProject);	//设置vp矩阵
		s_Data.m_Shader->SetFloat3("u_CameraPos", transform.m_Position);	//设置相机位置
		s_Data.m_Shader->SetFloat3("u_LightDir", { 1.0f, 1.0f, 1.0f });		//设置灯光照射方向
		s_Data.m_Shader->SetFloat3("u_LightColor", { 1.0f, 1.0f, 1.0f });	//设置相机位置
		s_Data.m_Shader->SetFloat3("u_AmbientColor", { 0.3f, 0.3f, 0.3f });	//设置环境光颜色
	}

	void Renderer3D::EndScene()
	{

	}
	
	void Renderer3D::DrawMesh(const Transform& transform)
	{
		s_Data.m_VertexArray->Bind();
		s_Data.m_Shader->Bind();

		glm::mat4 trans = glm::translate(glm::mat4(1.0f), transform.m_Position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(transform.m_Rotation.x), glm::vec3(1.0f, 0, 0))
			* glm::rotate(glm::mat4(1.0f), glm::radians(transform.m_Rotation.y), glm::vec3(0, 1.0f, 0))
			* glm::rotate(glm::mat4(1.0f), glm::radians(transform.m_Rotation.z), glm::vec3(0, 0, 1.0f))
			* glm::scale(glm::mat4(1.0f), transform.m_Scale);

		s_Data.m_Shader->SetMat4("u_Transform", trans);	//设置transform矩阵

		RenderCommand::DrawIndexed(s_Data.m_VertexArray, 36);
	}
}