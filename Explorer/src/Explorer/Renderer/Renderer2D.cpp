#include "exppch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Explorer
{
	/// <summary>
	/// �ı��ζ���
	/// </summary>
	struct QuadVertex
	{
		glm::vec3 Position;	//λ��
		glm::vec4 Color;	//��ɫ
		glm::vec2 TexCoord;	//��������
	};

	/// <summary>
	/// 2D��Ⱦ������
	/// </summary>
	struct Renderer2DData
	{
		uint32_t MaxQuads = 10000;			//����ı�������
		uint32_t MaxVertices = MaxQuads * 4;//��󶥵���
		uint32_t MaxIndices = MaxQuads * 6;	//�����������4������6��������

		std::shared_ptr<VertexArray> QuadVertexArray;	//�ı��ζ�������
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;				//�ı��ζ��㻺����
		std::shared_ptr<Shader>	TextureShader;			//������ɫ��
		std::shared_ptr<Texture2D>	WhiteTexture;		//��ɫ����

		uint32_t QuadIndexCount = 0;		//�ı�����������

		QuadVertex* QuadVertexBufferBase = nullptr;		//��������
		QuadVertex* QuadVertexBufferPtr = nullptr;
	};

	static Renderer2DData s_Data;	//��Ⱦ������

	void Renderer2D::Init()
	{
		s_Data.QuadVertexArray = std::make_shared<VertexArray>();		//���������������

		s_Data.QuadVertexBuffer = std::make_shared<VertexBuffer>(s_Data.MaxVertices * sizeof(QuadVertex));	//�������㻺��

		//���ö��㻺��������
		s_Data.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//λ��
			{ShaderDataType::Float4, "a_Color"},	//��ɫ
			{ShaderDataType::Float2, "a_Texture"},	//��������
		});
		s_Data.QuadVertexArray->AddVertexBuffer(s_Data.QuadVertexBuffer);	//���VBO��VAO

		s_Data.QuadVertexBufferBase = new QuadVertex[s_Data.MaxVertices];	//��������

		uint32_t* quadIndices = new uint32_t[s_Data.MaxIndices];	//��������

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndices; i += 6) {
			quadIndices[i + 0] = offset + 0;	//����
			quadIndices[i + 1] = offset + 1;	//����
			quadIndices[i + 2] = offset + 2;	//����

			quadIndices[i + 3] = offset + 2;	//����
			quadIndices[i + 4] = offset + 3;	//����
			quadIndices[i + 5] = offset + 0;	//����

			offset += 4;	//ƫ��4������
		}

		std::shared_ptr<IndexBuffer> quadIB = std::make_shared<IndexBuffer>(quadIndices, s_Data.MaxIndices);	//������������
		s_Data.QuadVertexArray->SetIndexBuffer(quadIB);									//����EBO��VAO
		delete[] quadIndices;

		s_Data.WhiteTexture = std::make_shared<Texture2D>(1, 1);			//�������Ϊ1������
		uint32_t whitTextureData = 0xffffffff;								//255��ɫ
		s_Data.WhiteTexture->SetData(&whitTextureData, sizeof(uint32_t));	//������������size = 1 * 1 * 4 == sizeof(uint32_t)

		s_Data.TextureShader = std::make_shared<Shader>("asserts/shaders/TextureShader");			//����Texture��ɫ��

		s_Data.TextureShader->Bind();					//��Texture��ɫ��
		s_Data.TextureShader->SetInt("u_Texture", 0);	//����texture����
	}

	void Renderer2D::Shutdown()
	{

	}

	void Renderer2D::BeginScene(const Camera& camera)
	{
		s_Data.TextureShader->Bind();		//��Texture��ɫ��
		s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());		//����vp����

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;	//��ʼ����������ָ��
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint32_t)s_Data.QuadVertexBufferPtr - (uint32_t)s_Data.QuadVertexBufferBase;	//���ݴ�С���ֽڣ�
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);	//���ö��㻺��������

		Flush();
	}

	void Renderer2D::Flush()
	{
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);	//����
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, { scale.x, scale.y, 1.0f }, color);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color)
	{
		//���½Ƕ������ݣ�ê�������½�
		s_Data.QuadVertexBufferPtr->Position = position;
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		//���½Ƕ�������
		s_Data.QuadVertexBufferPtr->Position = { position.x + scale.x, position.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 0.0f };
		s_Data.QuadVertexBufferPtr++;

		//���ϽǶ�������
		s_Data.QuadVertexBufferPtr->Position = { position.x + scale.x, position.y + scale.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 1.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		//���ϽǶ�������
		s_Data.QuadVertexBufferPtr->Position = { position.x, position.y + scale.y, 0.0f };
		s_Data.QuadVertexBufferPtr->Color = color;
		s_Data.QuadVertexBufferPtr->TexCoord = { 0.0f, 1.0f };
		s_Data.QuadVertexBufferPtr++;

		s_Data.QuadIndexCount += 6;	//������������

		/*s_Data.TextureShader->SetFloat4("u_Color", color);		//����color
		s_Data.TextureShader->SetFloat2("u_TilingFactor", { 1.0f, 1.0f });//���������ظ�����
		s_Data.WhiteTexture->Bind();							//�󶨰�ɫ����Ĭ������

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1.0f))
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		s_Data.TextureShader->SetMat4("u_Transform", transform);		//����transform����

		s_Data.QuadVertexArray->Bind();						//�󶨶�������
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);	//����*/
	}
	
	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, { scale.x, scale.y, 1.0f }, color, texture, tilingFactor);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tilingFactor)
	{
		s_Data.TextureShader->SetFloat4("u_Color", color);					//������ɫ��Ĭ����ɫ��
		s_Data.TextureShader->SetFloat2("u_TilingFactor", tilingFactor);	//���������ظ�����
		texture->Bind();													//������

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1.0f))
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		s_Data.TextureShader->SetMat4("u_Transform", transform);		//����transform����

		s_Data.QuadVertexArray->Bind();						//�󶨶�������
		RenderCommand::DrawIndexed(s_Data.QuadVertexArray);	//����
	}
}