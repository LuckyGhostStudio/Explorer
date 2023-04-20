#include "exppch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Explorer
{
	/// <summary>
	/// Sprite����
	/// </summary>
	struct SpriteVertex
	{
		glm::vec3 Position;	//λ��
		glm::vec4 Color;	//��ɫ
		glm::vec2 TexCoord;	//��������
		float TexIndex;		//��������

		int ObjectID;		//��������ID
	};

	/// <summary>
	/// 2D��Ⱦ������
	/// </summary>
	struct RendererData
	{
		static const uint32_t MaxSpritesCount = 10000;				//����ı�������
		static const uint32_t MaxVertexCount = MaxSpritesCount * 4;	//��󶥵���
		static const uint32_t MaxIndexCount = MaxSpritesCount * 6;	//�����������4������6��������
		static const uint32_t MaxTextureSlotCount = 32;			//����������

		std::shared_ptr<VertexArray> SpriteVertexArray;		//�ı��ζ�������
		std::shared_ptr<VertexBuffer> SpriteVertexBuffer;	//�ı��ζ��㻺����
		std::shared_ptr<Shader>	TextureShader;				//������ɫ��
		std::shared_ptr<Texture2D>	WhiteTexture;			//��ɫ����

		uint32_t SpriteIndexCount = 0;		//�ı���ʵ����������

		SpriteVertex* SpriteVertexBufferBase = nullptr;		//��������
		SpriteVertex* SpriteVertexBufferPtr = nullptr;

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlotCount> TextureSlots;	//������б� �洢����
		uint32_t TextureSlotIndex = 1;		//��������� 0 = white

		glm::vec4 SpriteVerticesPositions[4];

		Renderer2D::Statistics Stats;	//ͳ������
	};

	static RendererData s_Data;	//��Ⱦ������

	void Renderer2D::Init()
	{
		s_Data.SpriteVertexArray = std::make_shared<VertexArray>();		//���������������

		s_Data.SpriteVertexBuffer = std::make_shared<VertexBuffer>(s_Data.MaxVertexCount * sizeof(SpriteVertex));	//�������㻺��

		//���ö��㻺��������
		s_Data.SpriteVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},		//λ��
			{ShaderDataType::Float4, "a_Color"},		//��ɫ
			{ShaderDataType::Float2, "a_TexCoord"},		//��������
			{ShaderDataType::Float, "a_TexIndex"},		//��������
			{ShaderDataType::Int, "ObjectID"},			//��������ID
		});
		s_Data.SpriteVertexArray->AddVertexBuffer(s_Data.SpriteVertexBuffer);	//���VBO��VAO

		s_Data.SpriteVertexBufferBase = new SpriteVertex[s_Data.MaxVertexCount];	//��������

		uint32_t* SpriteIndices = new uint32_t[s_Data.MaxIndexCount];	//��������

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndexCount; i += 6) {
			SpriteIndices[i + 0] = offset + 0;	//����
			SpriteIndices[i + 1] = offset + 1;	//����
			SpriteIndices[i + 2] = offset + 2;	//����

			SpriteIndices[i + 3] = offset + 2;	//����
			SpriteIndices[i + 4] = offset + 3;	//����
			SpriteIndices[i + 5] = offset + 0;	//����

			offset += 4;	//ƫ��4������
		}

		std::shared_ptr<IndexBuffer> SpriteIB = std::make_shared<IndexBuffer>(SpriteIndices, s_Data.MaxIndexCount);	//������������
		s_Data.SpriteVertexArray->SetIndexBuffer(SpriteIB);									//����EBO��VAO
		delete[] SpriteIndices;

		s_Data.WhiteTexture = std::make_shared<Texture2D>(1, 1);			//�������Ϊ1������
		uint32_t whitTextureData = 0xffffffff;								//255��ɫ
		s_Data.WhiteTexture->SetData(&whitTextureData, sizeof(uint32_t));	//������������size = 1 * 1 * 4 == sizeof(uint32_t)

		int samplers[s_Data.MaxTextureSlotCount];	//��������� 0-31
		for (uint32_t i = 0; i < s_Data.MaxTextureSlotCount; i++) {
			samplers[i] = i;
		}
			
		s_Data.TextureShader = ShaderLibrary::Load("Sprite", "assets/shaders/TextureShader");	//����Texture��ɫ��

		s_Data.TextureShader->Bind();					//��Texture��ɫ��
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlotCount);	//����textures���� ���������

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;	//0�������Ϊ��ɫ����

		s_Data.SpriteVerticesPositions[0] = { -0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.SpriteVerticesPositions[1] = {  0.5f, -0.5f, 0.0f, 1.0f };
		s_Data.SpriteVerticesPositions[2] = {  0.5f,  0.5f, 0.0f, 1.0f };
		s_Data.SpriteVerticesPositions[3] = { -0.5f,  0.5f, 0.0f, 1.0f };
	}

	void Renderer2D::Shutdown()
	{

	}

	void Renderer2D::BeginScene(const EditorCamera& camera)
	{
		s_Data.TextureShader->Bind();	//��Texture��ɫ��
		s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//����vp����

		s_Data.SpriteIndexCount = 0;
		s_Data.SpriteVertexBufferPtr = s_Data.SpriteVertexBufferBase;	//��ʼ����������ָ��

		s_Data.TextureSlotIndex = 1;	//�����������1��ʼ 0Ϊ��ɫ����
	}

	void Renderer2D::BeginScene(const Camera& camera, const Transform& transform)
	{
		glm::mat4 viewProject = camera.GetProjection() * glm::inverse(transform.GetTransform());	//����VP���� vp = p * v

		s_Data.TextureShader->Bind();	//��Texture��ɫ��
		s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", viewProject);	//����vp����

		s_Data.SpriteIndexCount = 0;
		s_Data.SpriteVertexBufferPtr = s_Data.SpriteVertexBufferBase;	//��ʼ����������ָ��

		s_Data.TextureSlotIndex = 1;	//�����������1��ʼ 0Ϊ��ɫ����
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint32_t)s_Data.SpriteVertexBufferPtr - (uint32_t)s_Data.SpriteVertexBufferBase;	//���ݴ�С���ֽڣ�
		s_Data.SpriteVertexBuffer->SetData(s_Data.SpriteVertexBufferBase, dataSize);	//���ö��㻺��������

		Flush();
	}

	void Renderer2D::Flush()
	{
		//����������
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
			s_Data.TextureSlots[i]->Bind(i);	//��i�������
		}

		RenderCommand::DrawIndexed(s_Data.SpriteVertexArray, s_Data.SpriteIndexCount);	//����

		s_Data.Stats.DrawCalls++;	//���Ƶ��ô���++
	}

	void Renderer2D::DrawSprite(const Transform& transform, SpriteRenderer& spriteRenderer, int objectID)
	{
		s_Data.TextureShader->SetFloat4("u_Color", spriteRenderer.GetColor());	//������ɫuniform

		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };	//������ɫ
		auto& texture = spriteRenderer.GetSprite();
		//��������
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		////���������������������
		//if (s_Data.SpriteIndexCount >= RendererData::MaxIndexCount) {
		//	FlushAndReset();	//��ʼ��һ����Ⱦ
		//}
		//
		float textureIndex = 0.0f;	//��ǰ��������
		//
		////���������Ѵ��ڵ�����
		//for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
		//	if (*s_Data.TextureSlots[i].get() == *texture.get()) {	//texture���������
		//		textureIndex = (float)i;							//���õ�ǰ��������
		//		break;
		//	}
		//}
		//
		////��ǰ�������������
		//if (textureIndex == 0.0f) {
		//	textureIndex = (float)s_Data.TextureSlotIndex;
		//	s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;	//���texture�� �� s_Data.TextureSlotIndex�������
		//	s_Data.TextureSlotIndex++;	//���������++
		//}

		//4����������
		for (int i = 0; i < 4; i++) {
			s_Data.SpriteVertexBufferPtr->Position = transform.GetTransform() * s_Data.SpriteVerticesPositions[i];
			s_Data.SpriteVertexBufferPtr->Color = color;
			s_Data.SpriteVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.SpriteVertexBufferPtr->TexIndex = textureIndex;
			s_Data.SpriteVertexBufferPtr->ObjectID = objectID;

			s_Data.SpriteVertexBufferPtr++;
		}

		s_Data.SpriteIndexCount += 6;	//������������

		s_Data.Stats.SpriteCount++;	//�ı��θ���++
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();	//������һ����Ⱦ

		s_Data.SpriteIndexCount = 0;
		s_Data.SpriteVertexBufferPtr = s_Data.SpriteVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawSprite(const Transform& transform, const glm::vec4& color)
	{
		//���������������������
		if (s_Data.SpriteIndexCount >= RendererData::MaxIndexCount) {
			FlushAndReset();	//��ʼ��һ����Ⱦ
		}

		int SpriteVertexCount = 4;	//�������
		const glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };	//��������

		const float textureIndex = 0.0f;				//��ɫ��������
		const glm::vec2 tilingFactor = { 1.0f, 1.0f };	//����ƽ������

		//4����������
		for (int i = 0; i < SpriteVertexCount; i++) {
			s_Data.SpriteVertexBufferPtr->Position = transform.GetTransform() * s_Data.SpriteVerticesPositions[i];
			s_Data.SpriteVertexBufferPtr->Color = color;
			s_Data.SpriteVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.SpriteVertexBufferPtr->TexIndex = textureIndex;
			s_Data.SpriteVertexBufferPtr++;
		}

		s_Data.SpriteIndexCount += 6;	//������������

		s_Data.Stats.SpriteCount++;	//�ı��θ���++
	}
	
	void Renderer2D::DrawSprite(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tilingFactor)
	{
		//���������������������
		if (s_Data.SpriteIndexCount >= RendererData::MaxIndexCount) {
			FlushAndReset();	//��ʼ��һ����Ⱦ
		}

		int SpriteVertexCount = 4;	//�������
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
		for (int i = 0; i < SpriteVertexCount; i++) {
			s_Data.SpriteVertexBufferPtr->Position = transform * s_Data.SpriteVerticesPositions[i];
			s_Data.SpriteVertexBufferPtr->Color = color;
			s_Data.SpriteVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.SpriteVertexBufferPtr->TexIndex = textureIndex;
			s_Data.SpriteVertexBufferPtr++;
		}

		s_Data.SpriteIndexCount += 6;	//������������

		s_Data.Stats.SpriteCount++;	//�ı��θ���++
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const glm::vec4& color)
	{
		//���������������������
		if (s_Data.SpriteIndexCount >= RendererData::MaxIndexCount) {
			FlushAndReset();	//��ʼ��һ����Ⱦ
		}

		int SpriteVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		const float textureIndex = 0.0f;	//��ɫ��������
		const glm::vec2 tilingFactor = { 1.0f, 1.0f };	//����ƽ������

		//4����������
		for (int i = 0; i < SpriteVertexCount; i++) {
			s_Data.SpriteVertexBufferPtr->Position = transform * s_Data.SpriteVerticesPositions[i];
			s_Data.SpriteVertexBufferPtr->Color = color;
			s_Data.SpriteVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.SpriteVertexBufferPtr->TexIndex = textureIndex;
			s_Data.SpriteVertexBufferPtr++;
		}

		s_Data.SpriteIndexCount += 6;	//������������

		s_Data.Stats.SpriteCount++;	//�ı��θ���++
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tilingFactor, const glm::vec4& tintColor)
	{
		int SpriteVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		//��������
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		//���������������������
		if (s_Data.SpriteIndexCount >= RendererData::MaxIndexCount) {
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
		for (int i = 0; i < SpriteVertexCount; i++) {
			s_Data.SpriteVertexBufferPtr->Position = transform * s_Data.SpriteVerticesPositions[i];
			s_Data.SpriteVertexBufferPtr->Color = color;
			s_Data.SpriteVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.SpriteVertexBufferPtr->TexIndex = textureIndex;
			s_Data.SpriteVertexBufferPtr++;
		}

		s_Data.SpriteIndexCount += 6;	//������������

		s_Data.Stats.SpriteCount++;	//�ı��θ���++
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