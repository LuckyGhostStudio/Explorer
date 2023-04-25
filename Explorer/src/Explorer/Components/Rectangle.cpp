#include "exppch.h"
#include "Rectangle.h"

namespace Explorer
{
	Rectangle::Rectangle()
	{
		//顶点数据 -------------------位置-------- -----------颜色----------- objID
		m_Vertices[0] = { { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0 };	//左下
		m_Vertices[1] = { {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0 };	//右下
		m_Vertices[2] = { {  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0 };	//右上
		m_Vertices[3] = { { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0 };	//左上

		m_VertexArray = std::make_shared<VertexArray>();							//创建顶点数组VAO
		m_VertexBuffer = std::make_shared<VertexBuffer>(4 * sizeof(LineVertex));	//创建顶点缓冲VBO

		//设置顶点缓冲区布局
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//位置
			{ShaderDataType::Float4, "a_Color"},	//颜色
			{ShaderDataType::Int, "a_ObjectID"},	//物体ID
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);	//添加VBO到VAO

		m_VertexBufferData.reserve(4);	//预留顶点缓冲区数据空间
	}
	
	Rectangle::~Rectangle()
	{
		m_VertexBufferData.clear();
	}
}