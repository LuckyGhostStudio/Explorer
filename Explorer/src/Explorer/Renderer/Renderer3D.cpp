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
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,	// A 0
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,	// A 1
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,	// A 2

			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,	// B 3
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,	// B 4
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,	// B 5

			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,	// C 6
			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,	// C 7
			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,	// C 8

			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,	// D 9
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,	// D 10
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,	// D 11

			-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,	// E 12
			-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,	// E 13
			-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,	// E 14

			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,	// F 15
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,	// F 16
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,	// F 17

			-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,	// G 18
			-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,	// G 19
			-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,	// G 20

			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,	// H 21
			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,	// H 22
			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,	// H 23
		};

		s_Data.m_VertexBuffer = std::make_shared<VertexBuffer>(cubeVertices, 8 * 3 * 10 * sizeof(float));	//创建VBO

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
			0, 3, 6,	// A B C x+
			6, 9, 0,	// C D A x+
			18, 15, 12,	// G F E x-
			18, 21, 12,	// G H E x-
			22, 7, 19,	// H C G y+
			7, 10, 22,	// C D H y+
			13, 16, 4,	// E F B y-
			4, 1, 13,	// B A E y-
			23, 14, 2,	// H E A z+
			2, 11, 23,	// A D H z+
			20, 5, 17,	// G B F z-
			5, 20, 8,	// B G C z-
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
		glm::mat4 viewProject = camera.GetProjection() * glm::inverse(transform.GetTransform());	//vp = p * v

		s_Data.m_Shader->Bind();			//绑定着色器
		s_Data.m_Shader->SetMat4("u_ViewProjectionMatrix", viewProject);	//设置vp矩阵
		s_Data.m_Shader->SetFloat3("u_CameraPos", transform.m_Position);	//设置相机位置
		s_Data.m_Shader->SetFloat3("u_LightPos", { 10.0f, 10.0f, 10.0f });	//设置灯光位置
		s_Data.m_Shader->SetFloat3("u_LightColor", { 1.0f, 1.0f, 1.0f });	//设置灯光颜色
		s_Data.m_Shader->SetFloat3("u_AmbientColor", { 0.2f, 0.2f, 0.2f });	//设置环境光颜色
	}

	void Renderer3D::EndScene()
	{

	}
	
	void Renderer3D::DrawMesh(const Transform& transform)
	{
		s_Data.m_VertexArray->Bind();
		s_Data.m_Shader->Bind();

		s_Data.m_Shader->SetMat4("u_Transform", transform.GetTransform());	//设置transform矩阵

		RenderCommand::DrawIndexed(s_Data.m_VertexArray, 36);
	}
}