#include "exppch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Explorer
{
	/// <summary>
	/// 2D��Ⱦ������
	/// </summary>
	struct Renderer2DData
	{
		std::shared_ptr<VertexArray> QuadVertexArray;	//�ı��ζ�������
		std::shared_ptr<Shader>	FlatColorShader;		//�ı�����ɫ��
		std::shared_ptr<Shader> TextureShader;			//������ɫ��
	};

	static Renderer2DData* s_Data;	//��Ⱦ������

	void Renderer2D::Init()
	{
		s_Data = new Renderer2DData();

		s_Data->QuadVertexArray = std::make_shared<VertexArray>();		//���������������

		float squareVertices[] = {
			//------λ��------  --��������--
			-0.5f, -0.5f, 0.0f, 0.0f, 0.0f,	//����0
			 0.5f, -0.5f, 0.0f, 1.0f, 0.0f,	//����1
			-0.5f,  0.5f, 0.0f, 0.0f, 1.0f,	//����2
			 0.5f,  0.5f, 0.0f, 1.0f, 1.0f	//����3
		};

		std::shared_ptr<VertexBuffer> squareVB;										//VBO
		squareVB.reset(new VertexBuffer(squareVertices, sizeof(squareVertices)));	//�������㻺��

		//���ö��㻺��������
		squareVB->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//λ��
			{ShaderDataType::Float2, "a_Texture"},	//��������
		});
		s_Data->QuadVertexArray->AddVertexBuffer(squareVB);	//���VBO��VAO

		unsigned int squareIndices[] = { 0, 1, 2, 2, 1, 3 };		//��������

		std::shared_ptr<IndexBuffer> squareIB;														//EBO
		squareIB.reset(new IndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));	//������������
		s_Data->QuadVertexArray->SetIndexBuffer(squareIB);											//����EBO��VAO

		s_Data->FlatColorShader = std::make_shared<Shader>("asserts/shaders/FlatColorShader");		//������ɫ��
		s_Data->TextureShader = std::make_shared<Shader>("asserts/shaders/TextureShader");			//����Texture��ɫ��

		s_Data->TextureShader->Bind();					//��Texture��ɫ��
		s_Data->TextureShader->SetInt("u_Texture", 0);	//����texture����
	}

	void Renderer2D::Shutdown()
	{
		delete s_Data;	//�ͷ���Ⱦ������
	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data->FlatColorShader->Bind();	//����ɫ��
		s_Data->FlatColorShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//����vp����

		s_Data->TextureShader->Bind();		//��Texture��ɫ��
		s_Data->TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());		//����vp����
	}

	void Renderer2D::EndScene()
	{

	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, { scale.x, scale.y, 1.0f }, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color)
	{
		s_Data->FlatColorShader->Bind();						//����ɫ��
		s_Data->FlatColorShader->SetFloat4("u_Color", color);	//����color

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1.0f))
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		s_Data->FlatColorShader->SetMat4("u_Transform", transform);		//����transform����

		s_Data->QuadVertexArray->Bind();						//�󶨶�������
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);	//����
	}
	
	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, { scale.x, scale.y, 1.0f }, color, texture);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture)
	{
		s_Data->TextureShader->Bind();						//����ɫ��
		s_Data->TextureShader->SetFloat4("u_Color", color);	//����color

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1.0f))
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		s_Data->TextureShader->SetMat4("u_Transform", transform);		//����transform����

		texture->Bind();	//�������

		s_Data->QuadVertexArray->Bind();						//�󶨶�������
		RenderCommand::DrawIndexed(s_Data->QuadVertexArray);	//����
	}
}