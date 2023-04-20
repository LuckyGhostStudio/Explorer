#include "exppch.h"
#include "Sprite.h"

namespace Explorer
{
	Sprite::Sprite(const Vertex* vertices, const uint32_t* indices)
	{
		for (int i = 0; i < 4; i++) {
			m_Vertices[i] = vertices[i];
		}

		for (int i = 0; i < 6; i++) {
			m_VertexIndices[i] = indices[i];
		}

		m_VertexArray = std::make_shared<VertexArray>();						//创建顶点数组VAO
		m_VertexBuffer = std::make_shared<VertexBuffer>(4 * sizeof(Vertex));	//创建顶点缓冲VBO

		//设置顶点缓冲区布局
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//位置
			{ShaderDataType::Float4, "a_Color"},	//颜色
			{ShaderDataType::Float3, "a_Normal"},	//法线
			{ShaderDataType::Float2, "a_TexCoord"},	//纹理坐标
			{ShaderDataType::Int, "a_ID"},			//ID
			{ShaderDataType::Int, "a_ObjectID"},	//物体ID
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);	//添加VBO到VAO

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(m_VertexIndices, 6);	//创建索引缓冲EBO
		m_VertexArray->SetIndexBuffer(indexBuffer);												//设置EBO到VAO

		m_VertexBufferData.reserve(4);	//预留顶点缓冲区数据空间

		//--------------Texture---------------------------
		m_Texture = std::make_shared<Texture2D>(1, 1);				//创建宽高为1的纹理
		uint32_t whiteTextureData = 0xffffffff;						//255白色
		m_Texture->SetData(&whiteTextureData, sizeof(uint32_t));	//设置纹理数据size = 1 * 1 * 4 == sizeof(uint32_t)
	}

	Sprite::~Sprite()
	{
		m_VertexBufferData.clear();
	}
}