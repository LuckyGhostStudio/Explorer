#include "exppch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

namespace Explorer
{
	/// <summary>
	/// 2D渲染器数据
	/// </summary>
	struct Renderer2DData
	{
		std::shared_ptr<VertexArray> QuadVertexArray;	//四边形顶点数组
		std::shared_ptr<Shader>	FlatColorShader;		//四边形着色器
	};

	static Renderer2DData* s_Data;	//渲染器数据

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DData();

		s_Data->QuadVertexArray = std::make_shared<VertexArray>();		//创建顶点数组对象

		float squareVertices[] = {
			//------位置------  
			-0.5f, -0.5f, 0.0f,	//左下0
			 0.5f, -0.5f, 0.0f,	//右下1
			-0.5f,  0.5f, 0.0f,	//左上2
			 0.5f,  0.5f, 0.0f  //右上3
		};

		std::shared_ptr<VertexBuffer> squareVB;										//VBO
		squareVB.reset(new VertexBuffer(squareVertices, sizeof(squareVertices)));	//创建顶点缓冲

		//设置顶点缓冲区布局
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//位置
		});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);	//添加VBO到VAO

		unsigned int squareIndices[] = { 0, 1, 2, 2, 1, 3 };		//顶点索引

		std::shared_ptr<IndexBuffer> squareIB;														//EBO
		squareIB.reset(new IndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));	//创建索引缓冲
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);											//设置EBO到VAO

		s_Data->FlatColorShader = std::make_shared<Shader>("asserts/shaders/FlatColorShader");		//创建着色器
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;	//释放渲染器数据
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data->FlatColorShader->Bind();	//绑定着色器
		//设置uniform vp矩阵
		s_Data->FlatColorShader->UploadUniformMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
		//设置transform矩阵
		s_Data->FlatColorShader->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2 position, const glm::vec2 size, const glm::vec4 color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3 position, const glm::vec2 size, const glm::vec4 color)
	{
		s_Data->FlatColorShader->Bind();	//绑定着色器
		s_Data->FlatColorShader->UploadUniformFloat4("u_Color", color);	//设置uniform color

		s_Data->QuadVertexArray->Bind();						//绑定顶点数组
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);	//绘制
	}
}