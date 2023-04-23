#include "exppch.h"
#include "Sprite.h"

namespace Explorer
{
	Sprite::Sprite()
	{
		//顶点数据 -----------------位置---------  -----------颜色------------  --------法线--------  ---纹理坐标---  ID objID
		m_Vertices[0] = { { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }, 0, 0 };	//左下 0
		m_Vertices[1] = { {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }, 1, 0 };	//右下 1
		m_Vertices[2] = { {  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, 2, 0 };	//右上 2
		m_Vertices[3] = { { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }, 3, 0 };	//左上 3
		//顶点索引
		m_VertexIndices[0] = 0;
		m_VertexIndices[1] = 1;
		m_VertexIndices[2] = 2;
		m_VertexIndices[3] = 2;
		m_VertexIndices[4] = 3;
		m_VertexIndices[5] = 0;

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
		uint32_t whiteTextureData = 0x0eeeeeff;						//颜色
		m_Texture->SetData(&whiteTextureData, sizeof(uint32_t));	//设置纹理数据size = 1 * 1 * 4 == sizeof(uint32_t)
		m_TextureExist = false;										//纹理不存在
	}

	Sprite::~Sprite()
	{
		m_VertexBufferData.clear();
	}
	
	void Sprite::SetTexture(const std::shared_ptr<Texture2D>& texture)
	{
		m_Texture = texture;
		m_Texture->Bind();
		m_TextureExist = true;
	}
	
	void Sprite::SetTexture(const std::string& filepath)
	{
		SetTexture(std::make_shared<Texture2D>(filepath));
	}
}