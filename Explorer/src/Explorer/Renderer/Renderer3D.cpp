#include "exppch.h"
#include "Renderer3D.h"

#include "VertexArray.h"
#include "Shader.h"
#include "RenderCommand.h"

#include "Texture.h"
#include "Explorer/Components/Mesh.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glad/glad.h>	//TODO:待移除

namespace Explorer
{
	Renderer3D::RendererType Renderer3D::s_Type = RendererType::Rasterization;

	/// <summary>
	/// 渲染器数据
	/// </summary>
	struct RendererData
	{
		static const uint32_t MaxTextureSlotCount = 32;	//最大纹理槽数

		std::shared_ptr<Texture2D>	WhiteTexture;		//白色纹理 0号纹理

		uint32_t IndexCount = 0;						//当前已处理总索引个数

		std::array<std::shared_ptr<Texture2D>, MaxTextureSlotCount> TextureSlots;	//纹理槽列表 存储纹理
		uint32_t TextureSlotIndex = 1;					//当前纹理槽索引 0 = white

		uint32_t MaxLightCount = 24;	//最大光源数量
		uint32_t CurrentLightCount = 1;	//当前光源数量

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

		//s_Data.MeshShader = std::make_shared<Shader>("asserts/shaders/StandardShader");	//创建默认着色器
		//s_Data.MeshShader->Bind();														//绑定着色器
		//s_Data.MeshShader->SetIntArray("u_Textures", samplers, s_Data.MaxTextureSlotCount);	//设置textures变量 所有纹理槽

		s_Data.TextureSlots[0] = s_Data.WhiteTexture;	//0号纹理槽为白色纹理

		//----------------------------------------------
		ShaderLibrary::Load("Standard", "asserts/shaders/StandardShader");	//加载标准着色器
		ShaderLibrary::Load("Skybox", "asserts/shaders/SkyboxShader");		//加载天空盒着色器
	}

	void Renderer3D::Shutdown()
	{
		
	}

	void Renderer3D::BeginScene(const Camera& camera, Transform& transform)
	{
		if (camera.GetEnable()) {

		}
	}

	void Renderer3D::BeginScene(Environment& environment, const EditorCamera& camera, std::vector<Object>& lightObjects)
	{
		s_Data.CurrentLightCount = lightObjects.size();	//当前光源数量

		auto& standardShader = ShaderLibrary::Get("Standard");		//标准着色器
		standardShader->Bind();	//绑定标准着色器

		standardShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//设置vp矩阵
		standardShader->SetFloat3("u_CameraPos", camera.GetPosition());							//设置相机位置
		standardShader->SetInt("u_LightCount", s_Data.CurrentLightCount);						//光源数量

		//遍历Light对象列表
		for (int i = 0; i < s_Data.CurrentLightCount; i++) {
			Transform& lightTransform = lightObjects[i].GetComponent<Transform>();	//光源Transform
			Light& light = lightObjects[i].GetComponent<Light>();					//光源Light
			
			//设置Light Uniform数据
			light.SetShaderData(lightTransform.GetPosition(), -lightTransform.GetForwardDirection(), standardShader, i);
		}

		Skybox& skybox = environment.GetSkybox();	//天空盒
		skybox.GetCubemap()->Bind();				//绑定Cubemap纹理
		glm::mat4& rotationMatrix = glm::rotate(glm::mat4(1.0f), skybox.GetRotation(), { 0, 1, 0 });	//Skybox旋转矩阵（绕y轴）
		
		standardShader->SetInt("u_Environment.EnableSkybox", environment.GetSkyboxEnable());	//设置 是否启用天空盒
		standardShader->SetInt("u_Environment.Cubemap", 0);										//设置Cubemap
		standardShader->SetFloat3("u_Environment.TintColor", skybox.GetTintColor());			//设置Skybox色调
		standardShader->SetFloat("u_Environment.Expose", skybox.GetExpose());					//设置Skybox曝光度
		standardShader->SetMat4("u_Environment.SkyboxRotationMatrix", rotationMatrix);			//设置Skybox旋转矩阵（绕y轴）
		standardShader->SetInt("u_Environment.SourceType", (int)environment.GetSourceType());	//设置环境光源类型
		standardShader->SetFloat("u_Environment.IntensityMultiplier", environment.GetIntensityMultiplier());	//设置天空盒光强倍数
		standardShader->SetFloat3("u_Environment.AmbientColor", environment.GetAmbientColor());	//设置环境光颜色
	}

	void Renderer3D::EndScene(Environment& environment, const EditorCamera& camera)
	{
		//启用天空盒
		if (environment.GetSkyboxEnable()) {
			glDepthFunc(GL_LEQUAL);	//TODO:转移到RenderCommand
			Skybox& skybox = environment.GetSkybox();	//天空盒
			auto& skyboxShader = skybox.GetShader();	//Skybox着色器
			skyboxShader->Bind();						//绑定Skybox着色器
			skybox.GetCubemap()->Bind();				//绑定Cubemap纹理

			glm::mat4& rotationMatrix = glm::rotate(glm::mat4(1.0f), skybox.GetRotation(), { 0, 1, 0 });	//旋转矩阵（绕y轴）
			glm::mat4& viewMatrix = glm::mat4(glm::mat3(camera.GetViewMatrix()));		//移除view矩阵位移部分 使相机永远处于天空盒中心

			skyboxShader->SetMat4("u_RotationMatrix", rotationMatrix);					//设置旋转矩阵
			skyboxShader->SetMat4("u_ProjectionMatrix", camera.GetProjectionMatrix());	//设置projection矩阵
			skyboxShader->SetMat4("u_ViewMatrix", viewMatrix);							//设置view矩阵
			skyboxShader->SetInt("u_Cubemap", 0);										//设置Cubemap 0号槽位
			skyboxShader->SetFloat3("u_TintColor", skybox.GetTintColor());				//设置Skybox色调
			skyboxShader->SetFloat("u_Expose", skybox.GetExpose());						//设置Skybox曝光度

			RenderCommand::DrawIndexed(skybox.GetVertexArray());
			glDepthFunc(GL_LESS);	//TODO:转移到RenderCommand
		}
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
		material.SetShaderData();	//设置Material的Shader Uniform数据

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

			//Mesh组件启用
			if (mesh.GetEnable()) {
				SubMeshProcessing(subMesh);		//渲染子网格
			}
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