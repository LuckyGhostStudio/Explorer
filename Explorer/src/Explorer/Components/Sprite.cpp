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
		uint32_t whiteTextureData = 0xffffffff;						//255��ɫ
		m_Texture->SetData(&whiteTextureData, sizeof(uint32_t));	//������������size = 1 * 1 * 4 == sizeof(uint32_t)
	}

	Sprite::~Sprite()
	{
		m_VertexBufferData.clear();
	}
}