#include "exppch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

namespace Explorer
{
	/// <summary>
	/// 2D��Ⱦ������
	/// </summary>
	struct Renderer2DData
	{
		std::shared_ptr<VertexArray> QuadVertexArray;	//�ı��ζ�������
		std::shared_ptr<Shader>	FlatColorShader;		//�ı�����ɫ��
	};

	static Renderer2DData* s_Data;	//��Ⱦ������

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DData();

		s_Data->QuadVertexArray = std::make_shared<VertexArray>();		//���������������

		float squareVertices[] = {
			//------λ��------  
			-0.5f, -0.5f, 0.0f,	//����0
			 0.5f, -0.5f, 0.0f,	//����1
			-0.5f,  0.5f, 0.0f,	//����2
			 0.5f,  0.5f, 0.0f  //����3
		};

		std::shared_ptr<VertexBuffer> squareVB;										//VBO
		squareVB.reset(new VertexBuffer(squareVertices, sizeof(squareVertices)));	//�������㻺��

		//���ö��㻺��������
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//λ��
		});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);	//���VBO��VAO

		unsigned int squareIndices[] = { 0, 1, 2, 2, 1, 3 };		//��������

		std::shared_ptr<IndexBuffer> squareIB;														//EBO
		squareIB.reset(new IndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));	//������������
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);											//����EBO��VAO

		s_Data->FlatColorShader = std::make_shared<Shader>("asserts/shaders/FlatColorShader");		//������ɫ��
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;	//�ͷ���Ⱦ������
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data->FlatColorShader->Bind();	//����ɫ��
		//����uniform vp����
		s_Data->FlatColorShader->UploadUniformMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());
		//����transform����
		s_Data->FlatColorShader->UploadUniformMat4("u_Transform", glm::mat4(1.0f));
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2 position, const glm::vec2 size, const glm::vec4 color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, size, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3 position, const glm::vec2 size, const glm::vec4 color)
	{
		s_Data->FlatColorShader->Bind();	//����ɫ��
		s_Data->FlatColorShader->UploadUniformFloat4("u_Color", color);	//����uniform color

		s_Data->QuadVertexArray->Bind();						//�󶨶�������
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);	//����
	}
}