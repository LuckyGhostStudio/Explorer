#include "exppch.h"
#include "Rectangle.h"

namespace Explorer
{
	Rectangle::Rectangle()
	{
		//�������� -------------------λ��-------- -----------��ɫ----------- objID
		m_Vertices[0] = { { -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0 };	//����
		m_Vertices[1] = { {  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0 };	//����
		m_Vertices[2] = { {  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0 };	//����
		m_Vertices[3] = { { -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 0 };	//����

		m_VertexArray = std::make_shared<VertexArray>();							//������������VAO
		m_VertexBuffer = std::make_shared<VertexBuffer>(4 * sizeof(LineVertex));	//�������㻺��VBO

		//���ö��㻺��������
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//λ��
			{ShaderDataType::Float4, "a_Color"},	//��ɫ
			{ShaderDataType::Int, "a_ObjectID"},	//����ID
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);	//���VBO��VAO

		m_VertexBufferData.reserve(4);	//Ԥ�����㻺�������ݿռ�
	}
	
	Rectangle::~Rectangle()
	{
		m_VertexBufferData.clear();
	}
}