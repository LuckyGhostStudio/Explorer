#include "exppch.h"
#include "Circle.h"

namespace Explorer
{
	Circle::Circle()
	{
		//�������� ---------------����λ��--------  -------����λ��-------- -----------��ɫ------------	Thickness Fade objID
		m_Vertices[0] = { { -0.5f, -0.5f, 0.0f }, { -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//���� 0
		m_Vertices[1] = { {  0.5f, -0.5f, 0.0f }, {  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//���� 1
		m_Vertices[2] = { {  0.5f,  0.5f, 0.0f }, {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//���� 2
		m_Vertices[3] = { { -0.5f,  0.5f, 0.0f }, { -1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//���� 3
		//��������
		m_VertexIndices[0] = 0;
		m_VertexIndices[1] = 1;
		m_VertexIndices[2] = 2;
		m_VertexIndices[3] = 2;
		m_VertexIndices[4] = 3;
		m_VertexIndices[5] = 0;

		m_VertexArray = std::make_shared<VertexArray>();							//������������VAO
		m_VertexBuffer = std::make_shared<VertexBuffer>(4 * sizeof(CircleVertex));	//�������㻺��VBO

		//���ö��㻺��������
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_WorldPosition"},	//����λ��
			{ShaderDataType::Float3, "a_LocalPosition"},	//����λ��
			{ShaderDataType::Float4, "a_Color"},			//��ɫ
			{ShaderDataType::Float, "a_Thickness"},			//���
			{ShaderDataType::Float, "a_Fade"},				//ģ����
			{ShaderDataType::Int, "a_ObjectID"},			//����ID
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);	//���VBO��VAO

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(m_VertexIndices, 6);	//������������EBO
		m_VertexArray->SetIndexBuffer(indexBuffer);												//����EBO��VAO

		m_VertexBufferData.reserve(4);	//Ԥ�����㻺�������ݿռ�
	}

	Circle::Circle(float radius)
	{
		//�������� ---------------------����λ��--------  -------����λ��-------- -----------��ɫ------------Thickness Fade objID
		m_Vertices[0] = { { -radius, -radius, radius }, { -1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//���� 0
		m_Vertices[1] = { {  radius, -radius, radius }, {  1.0f, -1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//���� 1
		m_Vertices[2] = { {  radius,  radius, radius }, {  1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//���� 2
		m_Vertices[3] = { { -radius,  radius, radius }, { -1.0f,  1.0f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, 1.0f, 0.005f, 0 };	//���� 3
		//��������
		m_VertexIndices[0] = 0;
		m_VertexIndices[1] = 1;
		m_VertexIndices[2] = 2;
		m_VertexIndices[3] = 2;
		m_VertexIndices[4] = 3;
		m_VertexIndices[5] = 0;

		m_VertexArray = std::make_shared<VertexArray>();							//������������VAO
		m_VertexBuffer = std::make_shared<VertexBuffer>(4 * sizeof(CircleVertex));	//�������㻺��VBO

		//���ö��㻺��������
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_WorldPosition"},	//����λ��
			{ShaderDataType::Float3, "a_LocalPosition"},	//����λ��
			{ShaderDataType::Float4, "a_Color"},			//��ɫ
			{ShaderDataType::Float, "a_Thickness"},			//���
			{ShaderDataType::Float, "a_Fade"},				//ģ����
			{ShaderDataType::Int, "a_ObjectID"},			//����ID
			});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);	//���VBO��VAO

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(m_VertexIndices, 6);	//������������EBO
		m_VertexArray->SetIndexBuffer(indexBuffer);												//����EBO��VAO

		m_VertexBufferData.reserve(4);	//Ԥ�����㻺�������ݿռ�
	}

	Circle::~Circle()
	{
		m_VertexBufferData.clear();
	}
}