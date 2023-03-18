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
	/// 渲染器数据
	/// </summary>
	struct RendererData
	{
		static const uint32_t MaxTriangleCount = 12;	//最大三角形数量
		static const uint32_t MaxVertexCount = 8 * 3;	//最大顶点数:TODO:需修改
		static const uint32_t MaxIndexCount = 36;		//最大索引数
		static const uint32_t MaxTextureSlotCount = 32;		//最大纹理槽数

		std::shared_ptr<VertexArray> MeshVertexArray;	//顶点数组 VAO
		std::shared_ptr<VertexBuffer> MeshVertexBuffer;	//顶点缓冲区 VBO
		std::shared_ptr<IndexBuffer> MeshIndexBuffer;	//索引缓冲区 IBO
		std::shared_ptr<Shader>	MeshShader;				//着色器
		std::shared_ptr<Texture2D>	WhiteTexture;		//白色纹理 0号纹理

		Vertex* MeshVertexBufferBase = nullptr;			//顶点缓冲区数据初始地址
		Vertex* MeshVertexBufferPtr = nullptr;			//顶点缓冲区数据指针

		uint32_t IndexCount = 0;						//当前总索引个数

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlotCount> TextureSlots;	//纹理槽列表 存储纹理
		uint32_t TextureSlotIndex = 1;					//当前纹理槽索引 0 = white

		std::vector<Vertex> MeshVertexData;				//顶点数据

		Renderer3D::Statistics Stats;	//统计数据
	};

	static RendererData s_Data;	//渲染数据

	void Renderer3D::Init()
	{
		s_Data.MeshVertexArray = std::make_shared<VertexArray>();	//创建顶点数组VAO

		//正方体顶点
		float cubeVertices[] =
		{
            //-----位置坐标-----  ---------颜色---------  ------法向量------
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

		s_Data.MeshVertexBuffer = std::make_shared<VertexBuffer>(s_Data.MaxVertexCount * sizeof(Vertex));	//创建顶点缓冲VBO

		//设置顶点缓冲区布局
		s_Data.MeshVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//位置
			{ShaderDataType::Float4, "a_Color"},	//颜色
			{ShaderDataType::Float3, "a_Normal"},	//法线
			{ShaderDataType::Float2, "a_TexCoord"},	//纹理坐标
			{ShaderDataType::Float, "a_TexIndex"},	//纹理索引
			{ShaderDataType::Int, "a_ID"},			//ID
			{ShaderDataType::Int, "a_ObjectID"},	//物体ID
		});
		s_Data.MeshVertexArray->AddVertexBuffer(s_Data.MeshVertexBuffer);	//添加VBO到VAO

		s_Data.MeshVertexBufferBase = new Vertex[s_Data.MaxVertexCount];	//顶点缓冲区数据

		//顶点索引数据
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

		uint32_t* meshIndices = new uint32_t[s_Data.MaxIndexCount];		//顶点索引

		//设置顶点索引
		for (uint32_t i = 0; i < s_Data.MaxIndexCount; i++) {
			meshIndices[i] = cubeIndices[i % 36];
		}

		s_Data.MeshIndexBuffer = std::make_shared<IndexBuffer>(meshIndices, s_Data.MaxIndexCount);	//创建索引缓冲EBO
		s_Data.MeshVertexArray->SetIndexBuffer(s_Data.MeshIndexBuffer);								//设置EBO到VAO
		delete[] meshIndices;	//释放顶点索引数据

		s_Data.WhiteTexture = std::make_shared<Texture2D>(1, 1);			//创建宽高为1的纹理
		uint32_t whitTextureData = 0xffffffff;								//255白色
		s_Data.WhiteTexture->SetData(&whitTextureData, sizeof(uint32_t));	//设置纹理数据size = 1 * 1 * 4 == sizeof(uint32_t)

		int samplers[s_Data.MaxTextureSlotCount];		//纹理采样器 0-31
		for (uint32_t i = 0; i < s_Data.MaxTextureSlotCount; i++) {
			samplers[i] = i;
		}

		s_Data.MeshShader = std::make_shared<Shader>("asserts/shaders/StandardShader");	//创建默认着色器
		s_Data.MeshShader->Bind();														//绑定着色器
		//s_Data.MeshShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlotCount);	//设置textures变量 所有纹理槽

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;	//0号纹理槽为白色纹理

		//设置顶点数据
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

		s_Data.MeshShader->Bind();			//绑定着色器
		s_Data.MeshShader->SetMat4("u_ViewProjectionMatrix", viewProject);	//设置vp矩阵
		s_Data.MeshShader->SetFloat3("u_CameraPos", transform.m_Position);	//设置相机位置
		s_Data.MeshShader->SetFloat3("u_LightPos", { 10.0f, 10.0f, 10.0f });	//设置灯光位置
		s_Data.MeshShader->SetFloat3("u_LightColor", { 1.0f, 1.0f, 1.0f });	//设置灯光颜色
		s_Data.MeshShader->SetFloat3("u_AmbientColor", { 0.2f, 0.2f, 0.2f });	//设置环境光颜色

		StartBatchProcessing();	//开始批渲染
	}

	void Renderer3D::BeginScene(const EditorCamera& camera)
	{
		s_Data.MeshShader->Bind();			//绑定着色器
		s_Data.MeshShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//设置vp矩阵
		s_Data.MeshShader->SetFloat3("u_CameraPos", camera.GetPosition());		//设置相机位置
		s_Data.MeshShader->SetFloat3("u_LightPos", { 10.0f, 10.0f, 10.0f });	//设置灯光位置
		s_Data.MeshShader->SetFloat3("u_LightColor", { 1.0f, 1.0f, 1.0f });		//设置灯光颜色
		s_Data.MeshShader->SetFloat3("u_AmbientColor", { 0.2f, 0.2f, 0.2f });	//设置环境光颜色

		StartBatchProcessing();	//开始批渲染
	}

	void Renderer3D::StartBatchProcessing()
	{
		s_Data.IndexCount = 0;
		s_Data.MeshVertexBufferPtr = s_Data.MeshVertexBufferBase;	//初始化顶点数据指针：指向上次数据结尾

		s_Data.TextureSlotIndex = 1;	//纹理槽索引从1开始 0为白色纹理
	}

	void Renderer3D::EndScene()
	{
		BatchProcessing();	//批渲染过程处理
	}
	
	void Renderer3D::NextBatchProcessing()
	{
		BatchProcessing();		//批渲染过程处理
		StartBatchProcessing();	//开始下一批渲染：初始化渲染参数
	}

	void Renderer3D::BatchProcessing()
	{
		uint32_t dataSize = (uint32_t)s_Data.MeshVertexBufferPtr - (uint32_t)s_Data.MeshVertexBufferBase;	//数据大小（字节）= 数据指针 - 初始地址
		s_Data.MeshVertexBuffer->SetData(s_Data.MeshVertexBufferBase, dataSize);	//设置顶点缓冲区数据

		//绑定所有纹理
		for (uint32_t i = 0; i < s_Data.TextureSlotIndex; i++) {
			s_Data.TextureSlots[i]->Bind(i);	//绑定i号纹理槽
		}

		RenderCommand::DrawIndexed(s_Data.MeshVertexArray, s_Data.IndexCount);	//绘制调用

		s_Data.Stats.DrawCalls++;	//绘制调用次数++
	}

	void Renderer3D::DrawMesh(const Transform& transform, int objectID)
	{
		//当前索引个数超过最大索引数
		if (s_Data.IndexCount >= RendererData::MaxIndexCount) {
			NextBatchProcessing();	//开始新一批渲染
		}

		glm::mat4& transformMatrix = transform.GetTransform();

		//设置Mesh顶点缓冲区数据
		for (int i = 0; i < 24; i++) {
			s_Data.MeshVertexBufferPtr->Position = transformMatrix * glm::vec4(s_Data.MeshVertexData[i].Position, 1.0f);	//位置
			s_Data.MeshVertexBufferPtr->Color = s_Data.MeshVertexData[i].Color;			//颜色
			//法向量做 M 变换 M取逆矩阵的转置 防止normal在缩放时被拉伸
			s_Data.MeshVertexBufferPtr->Normal = glm::mat3(glm::transpose(glm::inverse(transformMatrix))) * s_Data.MeshVertexData[i].Normal;
			s_Data.MeshVertexBufferPtr->TexCoord = s_Data.MeshVertexData[i].TexCoord;	//纹理坐标
			s_Data.MeshVertexBufferPtr->TexIndex = s_Data.MeshVertexData[i].TexIndex;	//纹理索引
			
			s_Data.MeshVertexBufferPtr->ID = s_Data.MeshVertexData[i].ID;				//顶点ID
			s_Data.MeshVertexBufferPtr->ObjectID = objectID;	//物体ID

			s_Data.MeshVertexBufferPtr++;
		}

		s_Data.IndexCount += 36;	//索引个数增加

		s_Data.Stats.TriangleCount += 12;				//三角形个数
		s_Data.Stats.VertexCount += 8;					//累计顶点个数
		s_Data.Stats.IndexCount += 36;					//索引个数
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