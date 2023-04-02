#include "exppch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"
#include "RendererData.h"

#include "Texture.h"
#include "Explorer/Components/Mesh.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Explorer
{
	Renderer3D::RendererType Renderer3D::s_Type = RendererType::Rasterization;

	/// <summary>
	/// 渲染器数据
	/// </summary>
	struct RendererData
	{
		static const uint32_t MaxTextureSlotCount = 32;	//最大纹理槽数

		std::shared_ptr<ShaderLibrary> ShaderLibrary;	//着色器库
		std::shared_ptr<Shader>	MeshShader;				//着色器
		std::shared_ptr<Texture2D>	WhiteTexture;		//白色纹理 0号纹理

		uint32_t IndexCount = 0;						//当前已处理总索引个数

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlotCount> TextureSlots;	//纹理槽列表 存储纹理
		uint32_t TextureSlotIndex = 1;					//当前纹理槽索引 0 = white

		uint32_t MaxLightCount = 24;	//最大光源数量
		uint32_t CurrentLightCount = 1;	//当前光源数量

		LightData LightData;			//光照数据
		MaterialData MaterialData;		//材质数据
		Renderer3D::Statistics Stats;	//统计数据
	};

	static RendererData s_Data;	//渲染数据

	void Renderer3D::Init()
	{
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

		//----------------------------------------------
		//s_Data.ShaderLibrary->Load("asserts/shaders/StandardShader");	//加载着色器
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
	}

	void Renderer3D::BeginScene(const EditorCamera& camera, std::vector<Object>& lightObjects)
	{
		//TODO:对着色器库所有着色器进行设置
		s_Data.MeshShader->Bind();			//绑定着色器

		s_Data.MeshShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//设置vp矩阵
		s_Data.MeshShader->SetFloat3("u_CameraPos", camera.GetPosition());		//设置相机位置

		s_Data.CurrentLightCount = lightObjects.size();	//当前光源数量

		s_Data.MeshShader->SetInt("u_LightCount", s_Data.CurrentLightCount);	//光源数量

		//遍历Light对象列表
		for (int i = 0; i < s_Data.CurrentLightCount; i++) {
			Transform& lightTransform = lightObjects[i].GetComponent<Transform>();	//光源Transform
			Light& light = lightObjects[i].GetComponent<Light>();					//光源Light

			//设置Light数据
			s_Data.LightData.Position = lightTransform.m_Position;				//光源位置
			s_Data.LightData.Direction = -lightTransform.GetForwardDirection();	//光照方向 z-
			s_Data.LightData.Color = light.m_Color;								//光源颜色
			s_Data.LightData.Intensity = light.GetIntensity();					//光照强度
			s_Data.LightData.RenderShadow = light.m_RenderShadow;				//是否渲染阴影
			s_Data.LightData.Type = light.GetType();							//光照类型
			s_Data.LightData.Range = light.GetRange();							//光照半径
			s_Data.LightData.SpotOuterAngle = light.GetSpotOuterAngle();		//Spot外张角
			s_Data.LightData.SpotInnerAngle = light.GetSpotInnerAngle();		//Spot内张角

			//设置Light uniform变量
			s_Data.MeshShader->SetLightData(s_Data.LightData, i);
		}
		s_Data.MeshShader->SetFloat3("u_AmbientColor", { 0.2f, 0.2f, 0.2f });	//设置环境光颜色
	}

	void Renderer3D::EndScene()
	{
		
	}

	void Renderer3D::SubMeshProcessing(SubMesh& subMesh)
	{
		uint32_t dataSize = (uint32_t)sizeof(Vertex) * subMesh.GetVertexBufferData().size();	//计算顶点缓冲区数据大小（字节）

		subMesh.GetVertexBuffer()->SetData(subMesh.GetVertexBufferData().data(), dataSize);	//设置顶点缓冲区数据

		RenderCommand::DrawIndexed(subMesh.GetVertexArray());	//绘制调用

		s_Data.Stats.DrawCalls++;	//绘制调用次数++
	}

	void Renderer3D::DrawMesh(const Transform& transform, Mesh& mesh, Material& material, int objectID)
	{
		std::shared_ptr<Shader>& shader = material.GetShader();
		
		material.BindTexture();	//绑定纹理

		s_Data.MaterialData.Exist = true;												//材质是否存在:TODO:改变此属性改变材质组件存在性
		s_Data.MaterialData.Color = material.m_Color;									//颜色
		s_Data.MaterialData.AlbedoTextureExist = material.m_AlbedoTextureExist;			//反照率贴图是否存在
		s_Data.MaterialData.SpecularTextureExist = material.m_SpecularTextureExist;		//高光贴图是否存在
		s_Data.MaterialData.AlbedoTextureSlot = (int)Material::TextureType::Albedo;		//反照率贴图槽
		s_Data.MaterialData.SpecularTextureSlot = (int)Material::TextureType::Specular;	//高光贴图槽
		s_Data.MaterialData.Shininess = material.GetShininess();						//反光度
		s_Data.MaterialData.Tiling = material.m_Tiling;									//纹理平铺因子
		s_Data.MaterialData.Offset = material.m_Offset;									//纹理偏移量

		shader->SetMaterialData(s_Data.MaterialData);	//设置Material数据
		
		glm::mat4& transformMatrix = transform.GetTransform();

		//遍历所有子网格
		for (SubMesh& subMesh : mesh) {
			subMesh.GetVertexBufferData().clear();	//清空上一次SubMesh顶点缓冲区数据
			//遍历子网格所有顶点
			for (Vertex vertex : subMesh) {
				vertex.Position = transformMatrix * glm::vec4(vertex.Position, 1.0f);						//位置 做transform变换
				vertex.Normal = glm::mat3(glm::transpose(glm::inverse(transformMatrix))) * vertex.Normal;	//法向量做 M 变换 M取逆矩阵的转置 防止normal在缩放时被拉伸
				vertex.ObjectID = objectID;

				subMesh.GetVertexBufferData().push_back(vertex);	//添加顶点缓冲区数据
			}
			SubMeshProcessing(subMesh);		//渲染子网格
		}
		s_Data.IndexCount += mesh.GetVertexIndexCount();	//累计Mesh索引个数

		//s_Data.Stats.TriangleCount += 12;				//三角形个数
		s_Data.Stats.VertexCount += mesh.GetVertexCount();		//累计顶点个数
		s_Data.Stats.IndexCount += mesh.GetVertexIndexCount();	//累计顶点索引个数
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