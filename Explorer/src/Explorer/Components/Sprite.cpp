#include "exppch.h"
#include "Sprite.h"

namespace Explorer
{
	Sprite::Sprite()
	{
		//�������� -----------------λ��---------  -----------��ɫ------------  --------����--------  ---��������---  ID objID
		m_Vertices[0] = { { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }, 0, 0 };	//���� 0
		m_Vertices[1] = { {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }, 1, 0 };	//���� 1
		m_Vertices[2] = { {  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, 2, 0 };	//���� 2
		m_Vertices[3] = { { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }, 3, 0 };	//���� 3
		//��������
		m_VertexIndices[0] = 0;
		m_VertexIndices[1] = 1;
		m_VertexIndices[2] = 2;
		m_VertexIndices[3] = 2;
		m_VertexIndices[4] = 3;
		m_VertexIndices[5] = 0;

		m_VertexArray = std::make_shared<VertexArray>();						//������������VAO
		m_VertexBuffer = std::make_shared<VertexBuffer>(4 * sizeof(Vertex));	//�������㻺��VBO

		//���ö��㻺��������
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//λ��
			{ShaderDataType::Float4, "a_Color"},	//��ɫ
			{ShaderDataType::Float3, "a_Normal"},	//����
			{ShaderDataType::Float2, "a_TexCoord"},	//��������
			{ShaderDataType::Int, "a_ID"},			//ID
			{ShaderDataType::Int, "a_ObjectID"},	//����ID
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);	//���VBO��VAO

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(m_VertexIndices, 6);	//������������EBO
		m_VertexArray->SetIndexBuffer(indexBuffer);												//����EBO��VAO

		m_VertexBufferData.reserve(4);	//Ԥ�����㻺�������ݿռ�

		//--------------Texture---------------------------
		m_Texture = std::make_shared<Texture2D>(1, 1);				//�������Ϊ1������
		uint32_t whiteTextureData = 0x0eeeeeff;						//��ɫ
		m_Texture->SetData(&whiteTextureData, sizeof(uint32_t));	//������������size = 1 * 1 * 4 == sizeof(uint32_t)
		m_TextureExist = false;										//��������
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