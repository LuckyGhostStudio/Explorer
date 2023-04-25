#include "exppch.h"
#include "Line.h"

namespace Explorer
{
	Line::Line()
	{
		//�������� -------------------λ��-------- -----------��ɫ----------- objID
		m_Vertices[0] = { { -0.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0 };	//���
		m_Vertices[1] = { {  0.5f, 0.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0 };	//�յ�

		m_VertexArray = std::make_shared<VertexArray>();							//������������VAO
		m_VertexBuffer = std::make_shared<VertexBuffer>(4 * sizeof(LineVertex));	//�������㻺��VBO

		//���ö��㻺��������
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//λ��
			{ShaderDataType::Float4, "a_Color"},	//��ɫ
			{ShaderDataType::Int, "a_ObjectID"},	//����ID
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);	//���VBO��VAO

		m_VertexBufferData.reserve(2);	//Ԥ�����㻺�������ݿռ�
	}
	
	Line::Line(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int objectID)
	{
		//�������� -------λ��--��ɫ- --objID--
		m_Vertices[0] = { p0, color, objectID };	//���
		m_Vertices[1] = { p1, color, objectID };	//�յ�

		m_VertexArray = std::make_shared<VertexArray>();							//������������VAO
		m_VertexBuffer = std::make_shared<VertexBuffer>(4 * sizeof(LineVertex));	//�������㻺��VBO

		//���ö��㻺��������
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//λ��
			{ShaderDataType::Float4, "a_Color"},	//��ɫ
			{ShaderDataType::Int, "a_ObjectID"},	//����ID
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);	//���VBO��VAO

		m_VertexBufferData.reserve(2);	//Ԥ�����㻺�������ݿռ�
		//���ö�������
		m_VertexBufferData.push_back(m_Vertices[0]);
		m_VertexBufferData.push_back(m_Vertices[1]);
	}

	Line::~Line()
	{
		m_VertexBufferData.clear();
	}
}