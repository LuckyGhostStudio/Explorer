#include "exppch.h"
#include "Renderer2D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Explorer
{
	/// <summary>
	/// Sprite顶点
	/// </summary>
	struct SpriteVertex
	{
		glm::vec3 Position;	//位置
		glm::vec4 Color;	//颜色
		glm::vec2 TexCoord;	//纹理坐标
		float TexIndex;		//纹理索引

		int ObjectID;		//所属物体ID
	};

	/// <summary>
	/// 2D渲染器数据
	/// </summary>
	struct RendererData
	{
		static const uint32_t MaxSpritesCount = 10000;				//最大四边形数量
		static const uint32_t MaxVertexCount = MaxSpritesCount * 4;	//最大顶点数
		static const uint32_t MaxIndexCount = MaxSpritesCount * 6;	//最大索引数（4个顶点6个索引）
		static const uint32_t MaxTextureSlotCount = 32;			//最大纹理槽数

		std::shared_ptr<VertexArray> SpriteVertexArray;		//四边形顶点数组
		std::shared_ptr<VertexBuffer> SpriteVertexBuffer;	//四边形顶点缓冲区
		std::shared_ptr<Shader>	TextureShader;				//纹理着色器
		std::shared_ptr<Texture2D>	WhiteTexture;			//白色纹理

		uint32_t SpriteIndexCount = 0;		//四边形实际索引个数

		SpriteVertex* SpriteVertexBufferBase = nullptr;		//顶点数据
		SpriteVertex* SpriteVertexBufferPtr = nullptr;

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlotCount> TextureSlots;	//纹理槽列表 存储纹理
		uint32_t TextureSlotIndex = 1;		//纹理槽索引 0 = white

		glm::vec4 SpriteVerticesPositions[4];

		Renderer2D::Statistics Stats;	//统计数据
	};

	static RendererData s_Data;	//渲染器数据

	void Renderer2D::Init()
	{
		s_Data.SpriteVertexArray = std::make_shared<VertexArray>();		//创建顶点数组对象

		s_Data.SpriteVertexBuffer = std::make_shared<VertexBuffer>(s_Data.MaxVertexCount * sizeof(SpriteVertex));	//创建顶点缓冲

		//设置顶点缓冲区布局
		s_Data.SpriteVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},		//位置
			{ShaderDataType::Float4, "a_Color"},		//颜色
			{ShaderDataType::Float2, "a_TexCoord"},		//纹理坐标
			{ShaderDataType::Float, "a_TexIndex"},		//纹理索引
			{ShaderDataType::Int, "ObjectID"},			//所属物体ID
		});
		s_Data.SpriteVertexArray->AddVertexBuffer(s_Data.SpriteVertexBuffer);	//添加VBO到VAO

		s_Data.SpriteVertexBufferBase = new SpriteVertex[s_Data.MaxVertexCount];	//顶点数据

		uint32_t* SpriteIndices = new uint32_t[s_Data.MaxIndexCount];	//顶点索引

		uint32_t offset = 0;
		for (uint32_t i = 0; i < s_Data.MaxIndexCount; i += 6) {
			SpriteIndices[i + 0] = offset + 0;	//左下
			SpriteIndices[i + 1] = offset + 1;	//右下
			SpriteIndices[i + 2] = offset + 2;	//右上

			SpriteIndices[i + 3] = offset + 2;	//右上
			SpriteIndices[i + 4] = offset + 3;	//左上
			SpriteIndices[i + 5] = offset + 0;	//左下

			offset += 4;	//偏移4个顶点
		}

		std::shared_ptr<IndexBuffer> SpriteIB = std::make_shared<IndexBuffer>(SpriteIndices, s_Data.MaxIndexCount);	//创建索引缓冲
		s_Data.SpriteVertexArray->SetIndexBuffer(SpriteIB);									//设置EBO到VAO
		delete[] SpriteIndices;

		s_Data.WhiteTexture = std::make_shared<Texture2D>(1, 1);			//创建宽高为1的纹理
		uint32_t whitTextureData = 0xffffffff;								//255白色
		s_Data.WhiteTexture->SetData(&whitTextureData, sizeof(uint32_t));	//设置纹理数据size = 1 * 1 * 4 == sizeof(uint32_t)

		int samplers[s_Data.MaxTextureSlotCount];	//纹理采样器 0-31
		for (uint32_t i = 0; i < s_Data.MaxTextureSlotCount; i++) {
			samplers[i] = i;
		}
			
		s_Data.TextureShader = ShaderLibrary::Load("Sprite", "assets/shaders/TextureShader");	//创建Texture着色器

		s_Data.TextureShader->Bind();					//绑定Texture着色器
		s_Data.TextureShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlotCount);	//设置textures变量 所有纹理槽

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;	//0号纹理槽为白色纹理

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
		s_Data.TextureShader->Bind();	//绑定Texture着色器
		s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//设置vp矩阵

		s_Data.SpriteIndexCount = 0;
		s_Data.SpriteVertexBufferPtr = s_Data.SpriteVertexBufferBase;	//初始化顶点数据指针

		s_Data.TextureSlotIndex = 1;	//纹理槽索引从1开始 0为白色纹理
	}

	void Renderer2D::BeginScene(const Camera& camera, const Transform& transform)
	{
		glm::mat4 viewProject = camera.GetProjection() * glm::inverse(transform.GetTransform());	//计算VP矩阵 vp = p * v

		s_Data.TextureShader->Bind();	//绑定Texture着色器
		s_Data.TextureShader->SetMat4("u_ViewProjectionMatrix", viewProject);	//设置vp矩阵

		s_Data.SpriteIndexCount = 0;
		s_Data.SpriteVertexBufferPtr = s_Data.SpriteVertexBufferBase;	//初始化顶点数据指针

		s_Data.TextureSlotIndex = 1;	//纹理槽索引从1开始 0为白色纹理
	}

	void Renderer2D::EndScene()
	{
		uint32_t dataSize = (uint32_t)s_Data.SpriteVertexBufferPtr - (uint32_t)s_Data.SpriteVertexBufferBase;	//数据大小（字节）
		s_Data.SpriteVertexBuffer->SetData(s_Data.SpriteVertexBufferBase, dataSize);	//设置顶点缓冲区数据

		Flush();
	}

	void Renderer2D::Flush()
	{
		//绑定所有纹理
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
			s_Data.TextureSlots[i]->Bind(i);	//绑定i号纹理槽
		}

		RenderCommand::DrawIndexed(s_Data.SpriteVertexArray, s_Data.SpriteIndexCount);	//绘制

		s_Data.Stats.DrawCalls++;	//绘制调用次数++
	}

	void Renderer2D::DrawSprite(const Transform& transform, SpriteRenderer& spriteRenderer, int objectID)
	{
		s_Data.TextureShader->SetFloat4("u_Color", spriteRenderer.GetColor());	//设置颜色uniform

		glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };	//顶点颜色
		auto& texture = spriteRenderer.GetSprite();
		//纹理坐标
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		////索引个数超过最大索引数
		//if (s_Data.SpriteIndexCount >= RendererData::MaxIndexCount) {
		//	FlushAndReset();	//开始新一批渲染
		//}
		//
		float textureIndex = 0.0f;	//当前纹理索引
		//
		////遍历所有已存在的纹理
		//for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
		//	if (*s_Data.TextureSlots[i].get() == *texture.get()) {	//texture在纹理槽中
		//		textureIndex = (float)i;							//设置当前纹理索引
		//		break;
		//	}
		//}
		//
		////当前纹理不在纹理槽中
		//if (textureIndex == 0.0f) {
		//	textureIndex = (float)s_Data.TextureSlotIndex;
		//	s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;	//添加texture到 第 s_Data.TextureSlotIndex个纹理槽
		//	s_Data.TextureSlotIndex++;	//纹理槽索引++
		//}

		//4个顶点数据
		for (int i = 0; i < 4; i++) {
			s_Data.SpriteVertexBufferPtr->Position = transform.GetTransform() * s_Data.SpriteVerticesPositions[i];
			s_Data.SpriteVertexBufferPtr->Color = color;
			s_Data.SpriteVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.SpriteVertexBufferPtr->TexIndex = textureIndex;
			s_Data.SpriteVertexBufferPtr->ObjectID = objectID;

			s_Data.SpriteVertexBufferPtr++;
		}

		s_Data.SpriteIndexCount += 6;	//索引个数增加

		s_Data.Stats.SpriteCount++;	//四边形个数++
	}

	void Renderer2D::FlushAndReset()
	{
		EndScene();	//结束上一批渲染

		s_Data.SpriteIndexCount = 0;
		s_Data.SpriteVertexBufferPtr = s_Data.SpriteVertexBufferBase;

		s_Data.TextureSlotIndex = 1;
	}

	void Renderer2D::DrawSprite(const Transform& transform, const glm::vec4& color)
	{
		//索引个数超过最大索引数
		if (s_Data.SpriteIndexCount >= RendererData::MaxIndexCount) {
			FlushAndReset();	//开始新一批渲染
		}

		int SpriteVertexCount = 4;	//顶点个数
		const glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };	//纹理坐标

		const float textureIndex = 0.0f;				//白色纹理索引
		const glm::vec2 tilingFactor = { 1.0f, 1.0f };	//纹理平铺因子

		//4个顶点数据
		for (int i = 0; i < SpriteVertexCount; i++) {
			s_Data.SpriteVertexBufferPtr->Position = transform.GetTransform() * s_Data.SpriteVerticesPositions[i];
			s_Data.SpriteVertexBufferPtr->Color = color;
			s_Data.SpriteVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.SpriteVertexBufferPtr->TexIndex = textureIndex;
			s_Data.SpriteVertexBufferPtr++;
		}

		s_Data.SpriteIndexCount += 6;	//索引个数增加

		s_Data.Stats.SpriteCount++;	//四边形个数++
	}
	
	void Renderer2D::DrawSprite(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tilingFactor)
	{
		//索引个数超过最大索引数
		if (s_Data.SpriteIndexCount >= RendererData::MaxIndexCount) {
			FlushAndReset();	//开始新一批渲染
		}

		int SpriteVertexCount = 4;	//顶点个数
		const glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };	//纹理坐标

		float textureIndex = 0.0f;	//当前纹理索引

		//遍历所有已存在的纹理
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {	//texture在纹理槽中
				textureIndex = (float)i;							//设置当前纹理索引
				break;
			}
		}

		//当前纹理不在纹理槽中
		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;	//添加texture到 第 s_Data.TextureSlotIndex个纹理槽
			s_Data.TextureSlotIndex++;	//纹理槽索引++
		}

		glm::mat4 transform = glm::translate(glm::mat4(1.0f), position)
			* glm::rotate(glm::mat4(1.0f), glm::radians(rotation), glm::vec3(0, 0, 1.0f))
			* glm::scale(glm::mat4(1.0f), { scale.x, scale.y, 1.0f });

		//4个顶点数据
		for (int i = 0; i < SpriteVertexCount; i++) {
			s_Data.SpriteVertexBufferPtr->Position = transform * s_Data.SpriteVerticesPositions[i];
			s_Data.SpriteVertexBufferPtr->Color = color;
			s_Data.SpriteVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.SpriteVertexBufferPtr->TexIndex = textureIndex;
			s_Data.SpriteVertexBufferPtr++;
		}

		s_Data.SpriteIndexCount += 6;	//索引个数增加

		s_Data.Stats.SpriteCount++;	//四边形个数++
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const glm::vec4& color)
	{
		//索引个数超过最大索引数
		if (s_Data.SpriteIndexCount >= RendererData::MaxIndexCount) {
			FlushAndReset();	//开始新一批渲染
		}

		int SpriteVertexCount = 4;
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		const float textureIndex = 0.0f;	//白色纹理索引
		const glm::vec2 tilingFactor = { 1.0f, 1.0f };	//纹理平铺因子

		//4个顶点数据
		for (int i = 0; i < SpriteVertexCount; i++) {
			s_Data.SpriteVertexBufferPtr->Position = transform * s_Data.SpriteVerticesPositions[i];
			s_Data.SpriteVertexBufferPtr->Color = color;
			s_Data.SpriteVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.SpriteVertexBufferPtr->TexIndex = textureIndex;
			s_Data.SpriteVertexBufferPtr++;
		}

		s_Data.SpriteIndexCount += 6;	//索引个数增加

		s_Data.Stats.SpriteCount++;	//四边形个数++
	}

	void Renderer2D::DrawSprite(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tilingFactor, const glm::vec4& tintColor)
	{
		int SpriteVertexCount = 4;
		constexpr glm::vec4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		//纹理坐标
		constexpr glm::vec2 textureCoords[] = { { 0.0f, 0.0f }, { 1.0f, 0.0f }, { 1.0f, 1.0f }, { 0.0f, 1.0f } };

		//索引个数超过最大索引数
		if (s_Data.SpriteIndexCount >= RendererData::MaxIndexCount) {
			FlushAndReset();	//开始新一批渲染
		}

		float textureIndex = 0.0f;	//当前纹理索引

		//遍历所有已存在的纹理
		for (uint32_t i = 1; i < s_Data.TextureSlotIndex; i++) {
			if (*s_Data.TextureSlots[i].get() == *texture.get()) {	//texture在纹理槽中
				textureIndex = (float)i;							//设置当前纹理索引
				break;
			}
		}

		//当前纹理不在纹理槽中
		if (textureIndex == 0.0f) {
			textureIndex = (float)s_Data.TextureSlotIndex;
			s_Data.TextureSlots[s_Data.TextureSlotIndex] = texture;	//添加texture到 第 s_Data.TextureSlotIndex个纹理槽
			s_Data.TextureSlotIndex++;	//纹理槽索引++
		}

		//4个顶点数据
		for (int i = 0; i < SpriteVertexCount; i++) {
			s_Data.SpriteVertexBufferPtr->Position = transform * s_Data.SpriteVerticesPositions[i];
			s_Data.SpriteVertexBufferPtr->Color = color;
			s_Data.SpriteVertexBufferPtr->TexCoord = textureCoords[i];
			s_Data.SpriteVertexBufferPtr->TexIndex = textureIndex;
			s_Data.SpriteVertexBufferPtr++;
		}

		s_Data.SpriteIndexCount += 6;	//索引个数增加

		s_Data.Stats.SpriteCount++;	//四边形个数++
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