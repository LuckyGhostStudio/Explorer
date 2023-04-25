#include "exppch.h"
#include "Line.h"

namespace Explorer
{
	Line::Line()
	{
		//顶点数据 -------------------位置-------- -----------颜色----------- objID
		m_Vertices[0] = { { -0.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0 };	//起点
		m_Vertices[1] = { {  0.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0 };	//终点

		m_VertexArray = std::make_shared<VertexArray>();							//创建顶点数组VAO
		m_VertexBuffer = std::make_shared<VertexBuffer>(4 * sizeof(LineVertex));	//创建顶点缓冲VBO

		//设置顶点缓冲区布局
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//位置
			{ShaderDataType::Float4, "a_Color"},	//颜色
			{ShaderDataType::Int, "a_ObjectID"},	//物体ID
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);	//添加VBO到VAO

		m_VertexBufferData.reserve(2);	//预留顶点缓冲区数据空间
	}
	
	Line::Line(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int objectID)
	{
		//顶点数据 -------位置--颜色- --objID--
		m_Vertices[0] = { p0, color, objectID };	//起点
		m_Vertices[1] = { p1, color, objectID };	//终点

		m_VertexArray = std::make_shared<VertexArray>();							//创建顶点数组VAO
		m_VertexBuffer = std::make_shared<VertexBuffer>(4 * sizeof(LineVertex));	//创建顶点缓冲VBO

		//设置顶点缓冲区布局
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//位置
			{ShaderDataType::Float4, "a_Color"},	//颜色
			{ShaderDataType::Int, "a_ObjectID"},	//物体ID
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);	//添加VBO到VAO

		m_VertexBufferData.reserve(2);	//预留顶点缓冲区数据空间
		//设置顶点数据
		m_VertexBufferData.push_back(m_Vertices[0]);
		m_VertexBufferData.push_back(m_Vertices[1]);
	}

	Line::~Line()
	{
		m_VertexBufferData.clear();
	}
}