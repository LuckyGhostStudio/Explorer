#include "exppch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Texture.h"
#include "Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Explorer
{
	Renderer3D::RendererType Renderer3D::m_Type = RendererType::Rasterization;

	/// <summary>
	/// ��Ⱦ������
	/// </summary>
	struct RendererData
	{
		static const uint32_t MaxTriangleCount = 12;	//�������������
		static const uint32_t MaxVertexCount = 8 * 3;	//��󶥵���:TODO:���޸�
		static const uint32_t MaxIndexCount = 36;		//���������
		static const uint32_t MaxTextureSlotCount = 32;		//����������

		std::shared_ptr<VertexArray> MeshVertexArray;	//�������� VAO
		std::shared_ptr<VertexBuffer> MeshVertexBuffer;	//���㻺���� VBO
		std::shared_ptr<IndexBuffer> MeshIndexBuffer;	//���������� IBO
		std::shared_ptr<Shader>	MeshShader;				//��ɫ��
		std::shared_ptr<Texture2D>	WhiteTexture;		//��ɫ���� 0������

		Vertex* MeshVertexBufferBase = nullptr;			//���㻺�������ݳ�ʼ��ַ
		Vertex* MeshVertexBufferPtr = nullptr;			//���㻺��������ָ��

		uint32_t IndexCount = 0;						//��ǰ����������

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlotCount> TextureSlots;	//������б� �洢����
		uint32_t TextureSlotIndex = 1;					//��ǰ��������� 0 = white

		std::vector<Vertex> MeshVertexData;				//��������

		Renderer3D::Statistics Stats;	//ͳ������
	};

	static RendererData s_Data;	//��Ⱦ����

	void Renderer3D::Init()
	{
		s_Data.MeshVertexArray = std::make_shared<VertexArray>();	//������������VAO

		//�����嶥��
		float cubeVertices[] =
		{
            //-----λ������-----  ---------��ɫ---------  ------������------
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,	// A 0
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,	// A 1
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,	// A 2

			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,	// B 3
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,	// B 4
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,	// B 5

			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,	// C 6
			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,	// C 7
			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,	// C 8

			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,  0.0f,  0.0f,	// D 9
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,	// D 10
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,	// D 11

			-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,	// E 12
			-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,	// E 13
			-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,	// E 14

			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,	// F 15
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f, -1.0f,  0.0f,	// F 16
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,	// F 17

			-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,	// G 18
			-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,	// G 19
			-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f, -1.0f,	// G 20

			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  0.0f,  0.0f,	// H 21
			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  1.0f,  0.0f,	// H 22
			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  0.0f,  0.0f,  1.0f,	// H 23
		};

		s_Data.MeshVertexBuffer = std::make_shared<VertexBuffer>(s_Data.MaxVertexCount * sizeof(Vertex));	//�������㻺��VBO

		//���ö��㻺��������
		s_Data.MeshVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//λ��
			{ShaderDataType::Float4, "a_Color"},	//��ɫ
			{ShaderDataType::Float3, "a_Normal"},	//����
			{ShaderDataType::Float2, "a_TexCoord"},	//��������
			{ShaderDataType::Float, "a_TexIndex"},	//��������
			{ShaderDataType::Int, "a_ID"},			//ID
			{ShaderDataType::Int, "a_ObjectID"},	//����ID
		});
		s_Data.MeshVertexArray->AddVertexBuffer(s_Data.MeshVertexBuffer);	//���VBO��VAO

		s_Data.MeshVertexBufferBase = new Vertex[s_Data.MaxVertexCount];	//���㻺��������

		//������������
		uint32_t cubeIndices[] =
		{
			0, 3, 6,	// A B C x+
			6, 9, 0,	// C D A x+
			18, 15, 12,	// G F E x-
			18, 21, 12,	// G H E x-
			22, 7, 19,	// H C G y+
			7, 10, 22,	// C D H y+
			13, 16, 4,	// E F B y-
			4, 1, 13,	// B A E y-
			23, 14, 2,	// H E A z+
			2, 11, 23,	// A D H z+
			20, 5, 17,	// G B F z-
			5, 20, 8,	// B G C z-
		};

		uint32_t* meshIndices = new uint32_t[s_Data.MaxIndexCount];		//��������

		//���ö�������
		for (uint32_t i = 0; i < s_Data.MaxIndexCount; i++) {
			meshIndices[i] = cubeIndices[i % 36];
		}

		s_Data.MeshIndexBuffer = std::make_shared<IndexBuffer>(meshIndices, s_Data.MaxIndexCount);	//������������EBO
		s_Data.MeshVertexArray->SetIndexBuffer(s_Data.MeshIndexBuffer);								//����EBO��VAO
		delete[] meshIndices;	//�ͷŶ�����������

		s_Data.WhiteTexture = std::make_shared<Texture2D>(1, 1);			//�������Ϊ1������
		uint32_t whitTextureData = 0xffffffff;								//255��ɫ
		s_Data.WhiteTexture->SetData(&whitTextureData, sizeof(uint32_t));	//������������size = 1 * 1 * 4 == sizeof(uint32_t)

		int samplers[s_Data.MaxTextureSlotCount];		//��������� 0-31
		for (uint32_t i = 0; i < s_Data.MaxTextureSlotCount; i++) {
			samplers[i] = i;
		}

		s_Data.MeshShader = std::make_shared<Shader>("asserts/shaders/StandardShader");	//����Ĭ����ɫ��
		s_Data.MeshShader->Bind();														//����ɫ��
		//s_Data.MeshShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlotCount);	//����textures���� ���������

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;	//0�������Ϊ��ɫ����

		//���ö�������
		for (int i = 0; i < 24; i++) {
			Vertex vertex;
			
			vertex.Position = { cubeVertices[i * 10], cubeVertices[i * 10 + 1], cubeVertices[i * 10 + 2] };
			vertex.Color = { cubeVertices[i * 10 + 3], cubeVertices[i * 10 + 4], cubeVertices[i * 10 + 5], cubeVertices[i * 10 + 6] };
			vertex.Normal = { cubeVertices[i * 10 + 7], cubeVertices[i * 10 + 8], cubeVertices[i * 10 + 9] };
			vertex.TexCoord = {};
			vertex.TexIndex = 0.0f;
			
			vertex.ID = i;
			vertex.ObjectID = 0;

			s_Data.MeshVertexData.push_back(vertex);
		}
	}

	void Renderer3D::Shutdown()
	{

	}

	void Renderer3D::BeginScene(const Camera& camera, const Transform& transform)
	{
		glm::mat4 viewProject = camera.GetProjection() * glm::inverse(transform.GetTransform());	//vp = p * v

		s_Data.MeshShader->Bind();			//����ɫ��
		s_Data.MeshShader->SetMat4("u_ViewProjectionMatrix", viewProject);	//����vp����
		s_Data.MeshShader->SetFloat3("u_CameraPos", transform.m_Position);	//�������λ��
		s_Data.MeshShader->SetFloat3("u_LightPos", { 10.0f, 10.0f, 10.0f });	//���õƹ�λ��
		s_Data.MeshShader->SetFloat3("u_LightColor", { 1.0f, 1.0f, 1.0f });	//���õƹ���ɫ
		s_Data.MeshShader->SetFloat3("u_AmbientColor", { 0.2f, 0.2f, 0.2f });	//���û�������ɫ

		StartBatchProcessing();	//��ʼ����Ⱦ
	}

	void Renderer3D::BeginScene(const EditorCamera& camera)
	{
		s_Data.MeshShader->Bind();			//����ɫ��
		s_Data.MeshShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//����vp����
		s_Data.MeshShader->SetFloat3("u_CameraPos", camera.GetPosition());		//�������λ��
		s_Data.MeshShader->SetFloat3("u_LightPos", { 10.0f, 10.0f, 10.0f });	//���õƹ�λ��
		s_Data.MeshShader->SetFloat3("u_LightColor", { 1.0f, 1.0f, 1.0f });		//���õƹ���ɫ
		s_Data.MeshShader->SetFloat3("u_AmbientColor", { 0.2f, 0.2f, 0.2f });	//���û�������ɫ

		StartBatchProcessing();	//��ʼ����Ⱦ
	}

	void Renderer3D::StartBatchProcessing()
	{
		s_Data.IndexCount = 0;
		s_Data.MeshVertexBufferPtr = s_Data.MeshVertexBufferBase;	//��ʼ����������ָ�룺ָ���ϴ����ݽ�β

		s_Data.TextureSlotIndex = 1;	//�����������1��ʼ 0Ϊ��ɫ����
	}

	void Renderer3D::EndScene()
	{
		BatchProcessing();	//����Ⱦ���̴���
	}
	
	void Renderer3D::NextBatchProcessing()
	{
		BatchProcessing();		//����Ⱦ���̴���
		StartBatchProcessing();	//��ʼ��һ����Ⱦ����ʼ����Ⱦ����
	}

	void Renderer3D::BatchProcessing()
	{
		uint32_t dataSize = (uint32_t)s_Data.MeshVertexBufferPtr - (uint32_t)s_Data.MeshVertexBufferBase;	//���ݴ�С���ֽڣ�= ����ָ�� - ��ʼ��ַ
		s_Data.MeshVertexBuffer->SetData(s_Data.MeshVertexBufferBase, dataSize);	//���ö��㻺��������

		//����������
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
			s_Data.TextureSlots[i]->Bind(i);	//��i�������
		}

		RenderCommand::DrawIndexed(s_Data.MeshVertexArray, s_Data.IndexCount);	//���Ƶ���

		s_Data.Stats.DrawCalls++;	//���Ƶ��ô���++
	}

	void Renderer3D::DrawMesh(const Transform& transform, int objectID)
	{
		//��ǰ���������������������
		if (s_Data.IndexCount >= RendererData::MaxIndexCount) {
			NextBatchProcessing();	//��ʼ��һ����Ⱦ
		}

		glm::mat4& transformMatrix = transform.GetTransform();

		//����Mesh���㻺��������
		for (int i = 0; i < 24; i++) {
			s_Data.MeshVertexBufferPtr->Position = transformMatrix * glm::vec4(s_Data.MeshVertexData[i].Position, 1.0f);	//λ��
			s_Data.MeshVertexBufferPtr->Color = s_Data.MeshVertexData[i].Color;			//��ɫ
			//�������� M �任 Mȡ������ת�� ��ֹnormal������ʱ������
			s_Data.MeshVertexBufferPtr->Normal = glm::mat3(glm::transpose(glm::inverse(transformMatrix))) * s_Data.MeshVertexData[i].Normal;
			s_Data.MeshVertexBufferPtr->TexCoord = s_Data.MeshVertexData[i].TexCoord;	//��������
			s_Data.MeshVertexBufferPtr->TexIndex = s_Data.MeshVertexData[i].TexIndex;	//��������
			
			s_Data.MeshVertexBufferPtr->ID = s_Data.MeshVertexData[i].ID;				//����ID
			s_Data.MeshVertexBufferPtr->ObjectID = objectID;	//����ID

			s_Data.MeshVertexBufferPtr++;
		}

		s_Data.IndexCount += 36;	//������������

		s_Data.Stats.TriangleCount += 12;				//�����θ���
		s_Data.Stats.VertexCount += 8;					//�ۼƶ������
		s_Data.Stats.IndexCount += 36;					//��������
	}

	Renderer3D::Statistics Renderer3D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer3D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}
}