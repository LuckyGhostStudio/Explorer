#include "exppch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Explorer
{
	Renderer3D::RendererType Renderer3D::m_Type = RendererType::Rasterization;

	/// <summary>
	/// ��Ⱦ������
	/// </summary>
	struct RendererData
	{
		std::shared_ptr<VertexArray> m_VertexArray;		//�ı��ζ�������
		std::shared_ptr<VertexBuffer> m_VertexBuffer;	//�ı��ζ��㻺����
		std::shared_ptr<Shader>	m_Shader;				//������ɫ��
	};

	static RendererData s_Data;

	void Renderer3D::Init()
	{
		s_Data.m_VertexArray = std::make_shared<VertexArray>();	//����VAO

		//�����嶥��
		float cubeVertices[] =
		{
            //-----λ������-----  ---------��ɫ---------  ------������------
			 0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f, -1.0f,  1.0f,	// A 0
			 0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f, -1.0f, -1.0f,	// B 1
			 0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,  1.0f, -1.0f,	// C 2
			 0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f,  1.0f,  1.0f,  1.0f,	// D 3
			-0.5f, -0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f,  1.0f,	// E 4
			-0.5f, -0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f, -1.0f, -1.0f,	// F 5
			-0.5f,  0.5f, -0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  1.0f, -1.0f,	// G 6
			-0.5f,  0.5f,  0.5f, 1.0f, 0.0f, 1.0f, 1.0f, -1.0f,  1.0f,  1.0f,	// H 7
		};

		s_Data.m_VertexBuffer = std::make_shared<VertexBuffer>(cubeVertices, 8 * 10 * sizeof(float));	//����VBO

		//���ö��㻺��������
		s_Data.m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//λ��
			{ShaderDataType::Float4, "a_Color"},	//��ɫ
			{ShaderDataType::Float3, "a_Normal"},	//����
		});
		s_Data.m_VertexArray->AddVertexBuffer(s_Data.m_VertexBuffer);	//���VBO��VAO

		//��������
		uint32_t cubeIndices[] =
		{
			0, 1, 2,	// A B C
			2, 3, 0,	// C D A
			6, 5, 4,	// G F E
			4, 7, 6,	// G H E
			7, 2, 6,	// H C G
			2, 7, 3,	// C D H
			4, 5, 1,	// E F B
			1, 0, 4,	// B A E
			7, 4, 0,	// H E A
			0, 3, 7,	// A D H
			6, 1, 5,	// G B F
			1, 6, 2,	// B G C
		};

		std::shared_ptr<IndexBuffer> cubeEBO = std::make_shared<IndexBuffer>(cubeIndices, 36);	//����EBO
		s_Data.m_VertexArray->SetIndexBuffer(cubeEBO);											//����EBO��VAO

		s_Data.m_Shader = std::make_shared<Shader>("asserts/shaders/DefaultShader");	//����Ĭ����ɫ��
		s_Data.m_Shader->Bind();														//����ɫ��
	}

	void Renderer3D::Shutdown()
	{

	}

	void Renderer3D::BeginScene(const Camera& camera, const Transform& transform)
	{
		glm::mat4 viewProject = camera.GetProjection() * glm::inverse(transform.m_Transform);	//vp = p * v

		s_Data.m_Shader->Bind();			//����ɫ��
		s_Data.m_Shader->SetMat4("u_ViewProjectionMatrix", viewProject);	//����vp����
		s_Data.m_Shader->SetFloat3("u_CameraPos", transform.m_Position);	//�������λ��
		s_Data.m_Shader->SetFloat3("u_LightDir", { 1.0f, 1.0f, 1.0f });		//���õƹ����䷽��
		s_Data.m_Shader->SetFloat3("u_LightColor", { 1.0f, 1.0f, 1.0f });	//�������λ��
		s_Data.m_Shader->SetFloat3("u_AmbientColor", { 0.3f, 0.3f, 0.3f });	//���û�������ɫ
	}

	void Renderer3D::EndScene()
	{

	}
	
	void Renderer3D::DrawMesh(const Transform& transform)
	{
		s_Data.m_VertexArray->Bind();
		s_Data.m_Shader->Bind();

		glm::mat4 trans = glm::translate(glm::mat4(1.0f), transform.m_Position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(transform.m_Rotation.x), glm::vec3(1.0f, 0, 0))
			* glm::rotate(glm::mat4(1.0f), glm::radians(transform.m_Rotation.y), glm::vec3(0, 1.0f, 0))
			* glm::rotate(glm::mat4(1.0f), glm::radians(transform.m_Rotation.z), glm::vec3(0, 0, 1.0f))
			* glm::scale(glm::mat4(1.0f), transform.m_Scale);

		s_Data.m_Shader->SetMat4("u_Transform", trans);	//����transform����

		RenderCommand::DrawIndexed(s_Data.m_VertexArray, 36);
	}
}