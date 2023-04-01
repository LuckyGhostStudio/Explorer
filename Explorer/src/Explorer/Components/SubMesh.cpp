#include "exppch.h"
#include "SubMesh.h"

#include "Explorer/Renderer/RenderCommand.h"

namespace Explorer
{
	SubMesh::SubMesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices) 
		:m_Vertices(vertices), m_VertexIndices(indices)
	{
		m_VertexCount = m_Vertices.size();
		m_VertexIndexCount = m_VertexIndices.size();

		m_VertexArray = std::make_shared<VertexArray>();									//创建顶点数组VAO
		m_VertexBuffer = std::make_shared<VertexBuffer>(m_VertexCount * sizeof(Vertex));	//创建顶点缓冲VBO

		//设置顶点缓冲区布局
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//位置
			{ShaderDataType::Float4, "a_Color"},	//颜色
			{ShaderDataType::Float3, "a_Normal"},	//法线
			{ShaderDataType::Float2, "a_TexCoord"},	//纹理坐标
			//{ShaderDataType::Float, "a_TexIndex"},	//纹理索引
			{ShaderDataType::Int, "a_ID"},			//ID
			{ShaderDataType::Int, "a_ObjectID"},	//物体ID
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);	//添加VBO到VAO

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(m_VertexIndices.data(), m_VertexIndexCount);	//创建索引缓冲EBO
		m_VertexArray->SetIndexBuffer(indexBuffer);												//设置EBO到VAO

		m_VertexBufferData.reserve(m_VertexCount);	//预留顶点缓冲区数据空间
	}
	
	SubMesh::~SubMesh()
	{
		
	}
}