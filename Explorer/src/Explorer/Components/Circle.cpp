#include "exppch.h"
#include "Circle.h"

namespace Explorer
{
	Circle::Circle()
	{
		//顶点数据 ---------------世界位置--------  -------本地位置-------- -----------颜色------------	Thickness Fade objID
		m_Vertices[0] = { { -0.5f, -0.5f, 0.0f }, { -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//左下 0
		m_Vertices[1] = { {  0.5f, -0.5f, 0.0f }, {  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//右下 1
		m_Vertices[2] = { {  0.5f,  0.5f, 0.0f }, {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//右上 2
		m_Vertices[3] = { { -0.5f,  0.5f, 0.0f }, { -1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//左上 3
		//顶点索引
		m_VertexIndices[0] = 0;
		m_VertexIndices[1] = 1;
		m_VertexIndices[2] = 2;
		m_VertexIndices[3] = 2;
		m_VertexIndices[4] = 3;
		m_VertexIndices[5] = 0;

		m_VertexArray = std::make_shared<VertexArray>();							//创建顶点数组VAO
		m_VertexBuffer = std::make_shared<VertexBuffer>(4 * sizeof(CircleVertex));	//创建顶点缓冲VBO

		//设置顶点缓冲区布局
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_WorldPosition"},	//世界位置
			{ShaderDataType::Float3, "a_LocalPosition"},	//本地位置
			{ShaderDataType::Float4, "a_Color"},			//颜色
			{ShaderDataType::Float, "a_Thickness"},			//厚度
			{ShaderDataType::Float, "a_Fade"},				//模糊度
			{ShaderDataType::Int, "a_ObjectID"},			//物体ID
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);	//添加VBO到VAO

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(m_VertexIndices, 6);	//创建索引缓冲EBO
		m_VertexArray->SetIndexBuffer(indexBuffer);												//设置EBO到VAO

		m_VertexBufferData.reserve(4);	//预留顶点缓冲区数据空间
	}

	Circle::Circle(float radius)
	{
		//顶点数据 ---------------------世界位置--------  -------本地位置-------- -----------颜色------------Thickness Fade objID
		m_Vertices[0] = { { -radius, -radius, radius }, { -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//左下 0
		m_Vertices[1] = { {  radius, -radius, radius }, {  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//右下 1
		m_Vertices[2] = { {  radius,  radius, radius }, {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//右上 2
		m_Vertices[3] = { { -radius,  radius, radius }, { -1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//左上 3
		//顶点索引
		m_VertexIndices[0] = 0;
		m_VertexIndices[1] = 1;
		m_VertexIndices[2] = 2;
		m_VertexIndices[3] = 2;
		m_VertexIndices[4] = 3;
		m_VertexIndices[5] = 0;

		m_VertexArray = std::make_shared<VertexArray>();							//创建顶点数组VAO
		m_VertexBuffer = std::make_shared<VertexBuffer>(4 * sizeof(CircleVertex));	//创建顶点缓冲VBO

		//设置顶点缓冲区布局
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_WorldPosition"},	//世界位置
			{ShaderDataType::Float3, "a_LocalPosition"},	//本地位置
			{ShaderDataType::Float4, "a_Color"},			//颜色
			{ShaderDataType::Float, "a_Thickness"},			//厚度
			{ShaderDataType::Float, "a_Fade"},				//模糊度
			{ShaderDataType::Int, "a_ObjectID"},			//物体ID
			});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);	//添加VBO到VAO

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(m_VertexIndices, 6);	//创建索引缓冲EBO
		m_VertexArray->SetIndexBuffer(indexBuffer);												//设置EBO到VAO

		m_VertexBufferData.reserve(4);	//预留顶点缓冲区数据空间
	}

	Circle::~Circle()
	{
		m_VertexBufferData.clear();
	}
}