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

		m_VertexArray = std::make_shared<VertexArray>();									//������������VAO
		m_VertexBuffer = std::make_shared<VertexBuffer>(m_VertexCount * sizeof(Vertex));	//�������㻺��VBO

		//���ö��㻺��������
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//λ��
			{ShaderDataType::Float4, "a_Color"},	//��ɫ
			{ShaderDataType::Float3, "a_Normal"},	//����
			{ShaderDataType::Float2, "a_TexCoord"},	//��������
			//{ShaderDataType::Float, "a_TexIndex"},	//��������
			{ShaderDataType::Int, "a_ID"},			//ID
			{ShaderDataType::Int, "a_ObjectID"},	//����ID
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);	//���VBO��VAO

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(m_VertexIndices.data(), m_VertexIndexCount);	//������������EBO
		m_VertexArray->SetIndexBuffer(indexBuffer);												//����EBO��VAO

		m_VertexBufferData.reserve(m_VertexCount);	//Ԥ�����㻺�������ݿռ�
	}
	
	SubMesh::~SubMesh()
	{
		
	}
}