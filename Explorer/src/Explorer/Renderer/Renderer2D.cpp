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
		glm::vec3 Position;		//λ��
		glm::vec4 Color;		//��ɫ
		glm::vec2 TexCoord;		//��������
		float TexIndex;			//��������
		glm::vec2 TilingFactor;	//����ƽ������[x,y]
	};

	/// <summary>
	/// 2D��Ⱦ������
	/// </summary>
	struct Renderer2DData
	{
		static const uint32_t MaxQuads = 10000;					//����ı�������
		static const uint32_t MaxVertices = MaxQuads * 4;		//��󶥵���
		static const uint32_t MaxIndices = MaxQuads * 6;			//�����������4������6��������
		static const uint32_t MaxTextureSlots = 32;	//����������

		std::shared_ptr<VertexArray> QuadVertexArray;	//�ı��ζ�������
		std::shared_ptr<VertexBuffer> QuadVertexBuffer;	//�ı��ζ��㻺����
		std::shared_ptr<Shader>	TextureShader;			//������ɫ��
		std::shared_ptr<Texture2D>	WhiteTexture;		//��ɫ����

		uint32_t QuadIndexCount = 0;		//�ı�����������

		QuadVertex* QuadVertexBufferBase = nullptr;		//��������
		QuadVertex* QuadVertexBufferPtr = nullptr;

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlots> TextureSlots;	//������б� �洢����
		uint32_t TextureSlotIndex = 1;		//��������� 0 = white

		glm::vec4 QuadVerticesPositions[4];

		Renderer2D::Statistics Stats;	//ͳ������
	};

	static Renderer2DData s_Data;	//��Ⱦ������

	void Renderer2D::Init()
	{
		s_Data.QuadVertexArray = std::make_shared<VertexArray>();		//���������������

		s_Data.QuadVertexBuffer = std::make_shared<VertexBuffer>(s_Data.MaxVertices * sizeof(QuadVertex));	//�������㻺��

		//���ö��㻺��������
		s_Data.QuadVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},		//λ��
			{ShaderDataType::Float4, "a_Color"},		//��ɫ
			{ShaderDataType::Float2, "a_Texture"},		//��������
			{ShaderDataType::Float, "a_TexIndex"},		//��������
			{ShaderDataType::Float2, "a_TilingFactor"},	//����ƽ������
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

		int samplers[s_Data.MaxTextureSlots];	//��������� 0-31
		for (uint32_t i = 0; i < s_Data.MaxTextureSlots; i++) {
			samplers[i] = i;
		}

		s_Data.TextureShader = std::make_shared<Shader>("asserts/shaders/TextureShader");			//����Texture��ɫ��

		s_Data.TextureShader->Bind();					//��Texture��ɫ��
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlots);	//����textures���� ���������

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;	//0�������Ϊ��ɫ����

		s_Data.QuadVerticesPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVerticesPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.QuadVerticesPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.QuadVerticesPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{

	}

	void Renderer2D::BeginScene(const Camera& camera, Transform& transform)
	{
		transform.CalculateTransform();	//����Transform����
		glm::mat4 viewProject = camera.GetProjection() * glm::inverse((glm::mat4&)transform);	//����VP���� vp = p * v

		s_Data.TextureShader->Bind();	//��Texture��ɫ��
		s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", viewProject);	//����vp����

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;	//��ʼ����������ָ��

		s_Data.TextureSlotIndex = 1;	//�����������1��ʼ 0Ϊ��ɫ����
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint32_t)s_Data.QuadVertexBufferPtr - (uint32_t)s_Data.QuadVertexBufferBase;	//���ݴ�С���ֽڣ�
		s_Data.QuadVertexBuffer->SetData(s_Data.QuadVertexBufferBase, dataSize);	//���ö��㻺��������

		Flush();
	}

	void Renderer2D::Flush()
	{
		//����������
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
			s_Data.TextureSlots[i]->Bind(i);	//��i�������
		}

		RenderCommand::DrawIndexed(s_Data.QuadVertexArray, s_Data.QuadIndexCount);	//����

		s_Data.Stats.DrawCalls++;	//���Ƶ��ô���++
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();	//������һ����Ⱦ

		s_Data.QuadIndexCount = 0;
		s_Data.QuadVertexBufferPtr = s_Data.QuadVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color)
	{
		//DrawQuad({ position.x, position.y, 0.0f }, rotation, { scale.x, scale.y, 1.0f }, color);
	}

	void Renderer2D::DrawQuad(Transform& transform, const glm::vec4& color)
	{
		//���������������������
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();	//��ʼ��һ����Ⱦ
		}

		int quadVertexCount = 4;	//�������
		const glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };	//��������

		const float textureIndex = 0.0f;				//��ɫ��������
		const glm::vec2 tilingFactor = { 1.0f, 1.0f };	//����ƽ������

		transform.CalculateTransform();	//����Transform����

		//4����������
		for (int i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform.m_Transform * s_Data.QuadVerticesPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;	//������������

		s_Data.Stats.QuadCount++;	//�ı��θ���++
	}
	
	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, { scale.x, scale.y, 1.0f }, color, texture, tilingFactor);
	}
	
	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tilingFactor)
	{
		//���������������������
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();	//��ʼ��һ����Ⱦ
		}

		int quadVertexCount = 4;	//�������
		const glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };	//��������

		float textureIndex = 0.0f;	//��ǰ��������

		//���������Ѵ��ڵ�����
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {	//texture���������
				textureIndex = (float)i;							//���õ�ǰ��������
				break;
			}
		}

		//��ǰ�������������
		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;	//���texture�� �� s_Data.TextureSlotIndex�������
			s_Data.TextureSlotIndex++;	//���������++
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1.0f))
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		//4����������
		for (int i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVerticesPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;	//������������

		s_Data.Stats.QuadCount++;	//�ı��θ���++
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const glm::vec4& color)
	{
		//���������������������
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();	//��ʼ��һ����Ⱦ
		}

		int quadVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		const float textureIndex = 0.0f;	//��ɫ��������
		const glm::vec2 tilingFactor = { 1.0f, 1.0f };	//����ƽ������

		//4����������
		for (int i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVerticesPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;	//������������

		s_Data.Stats.QuadCount++;	//�ı��θ���++
	}

	void Renderer2D::DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tilingFactor, const glm::vec4& tintColor)
	{
		int quadVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		//��������
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		//���������������������
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();	//��ʼ��һ����Ⱦ
		}

		float textureIndex = 0.0f;	//��ǰ��������

		//���������Ѵ��ڵ�����
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {	//texture���������
				textureIndex = (float)i;							//���õ�ǰ��������
				break;
			}
		}

		//��ǰ�������������
		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;	//���texture�� �� s_Data.TextureSlotIndex�������
			s_Data.TextureSlotIndex++;	//���������++
		}

		//4����������
		for (int i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVerticesPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;	//������������

		s_Data.Stats.QuadCount++;	//�ı��θ���++
	}

	void Renderer2D::DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color, const std::shared_ptr<SubTexture2D>& subTexture, const glm::vec2& tilingFactor)
	{
		DrawQuad({ position.x, position.y, 0.0f }, rotation, { scale.x, scale.y, 1.0f }, color, subTexture, tilingFactor);
	}

	void Renderer2D::DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color, const std::shared_ptr<SubTexture2D>& subTexture, const glm::vec2& tilingFactor)
	{
		//���������������������
		if (s_Data.QuadIndexCount >= Renderer2DData::MaxIndices) {
			FlushAndReset();	//��ʼ��һ����Ⱦ
		}

		int quadVertexCount = 4;	//�������
		const glm::vec2* textureCoords = subTexture->GetTexCoords();	//����������
		std::shared_ptr<Texture2D> texture = subTexture->GetTexture();	//����

		float textureIndex = 0.0f;	//��ǰ��������

		//���������Ѵ��ڵ�����
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {	//texture���������
				textureIndex = (float)i;							//���õ�ǰ��������
				break;
			}
		}

		//��ǰ�������������
		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;	//���texture�� �� s_Data.TextureSlotIndex�������
			s_Data.TextureSlotIndex++;	//���������++
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1.0f))
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		//4����������
		for (int i = 0; i < quadVertexCount; i++) {
			s_Data.QuadVertexBufferPtr->Position = transform * s_Data.QuadVerticesPositions[i];
			s_Data.QuadVertexBufferPtr->Color = color;
			s_Data.QuadVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.QuadVertexBufferPtr->TexIndex = textureIndex;
			s_Data.QuadVertexBufferPtr->TilingFactor = tilingFactor;
			s_Data.QuadVertexBufferPtr++;
		}

		s_Data.QuadIndexCount += 6;	//������������

		s_Data.Stats.QuadCount++;	//�ı��θ���++
	}

	Renderer2D::Statistics Renderer2D::GetStats()
	{
		return s_Data.Stats;
	}

	void Renderer2D::ResetStats()
	{
		memset(&s_Data.Stats, 0, sizeof(Statistics));
	}
}