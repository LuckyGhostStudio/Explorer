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
		uint32_t MaxLightCount = 24;	//最大光源数量
		uint32_t CurrentLightCount = 1;	//当前光源数量

		Renderer3D::Statistics Stats;	//统计数据
	};

	static RendererData s_Data;	//渲染数据

	void Renderer3D::Init()
	{
		ShaderLibrary::Load("Standard", "assets/shaders/StandardShader");	//加载标准着色器
		ShaderLibrary::Load("Skybox", "assets/shaders/SkyboxShader");		//加载天空盒着色器
		ShaderLibrary::Load("Sprite", "assets/shaders/SpriteShader");		//加载Sprite着色器
		ShaderLibrary::Load("Circle", "assets/shaders/CircleShader");		//加载Circle着色器
		ShaderLibrary::Load("Line", "assets/shaders/LineShader");			//加载Line着色器
	}

	void Renderer3D::Shutdown()
	{
		
	}

	void Renderer3D::BeginScene(const EditorCamera& camera)
	{
		//-------------------Sprite-----------------------------------
		auto& spriteShader = ShaderLibrary::Get("Sprite");	//Sprite着色器
		spriteShader->Bind();								//绑定着色器

		spriteShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//设置vp矩阵

		//-------------------Circle-----------------------------------
		auto& circleShader = ShaderLibrary::Get("Circle");	//Circle着色器
		circleShader->Bind();								//绑定着色器

		circleShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//设置vp矩阵

		//-------------------Line-----------------------------------
		auto& lineShader = ShaderLibrary::Get("Line");	//Line着色器
		lineShader->Bind();								//绑定着色器

		lineShader->SetMat4("u_ViewProjectionMatrix", camera.GetViewProjectionMatrix());	//设置vp矩阵
	}

	void Renderer3D::BeginScene(const Camera& camera, Transform& transform)
	{
		//-------------------Sprite-----------------------------------
		auto& spriteShader = ShaderLibrary::Get("Sprite");	//Sprite着色器
		spriteShader->Bind();	//绑定着色器

		glm::mat4& viewProjectionMatrix = camera.GetProjection() * glm::inverse(transform.GetTransform());	//计算VP矩阵 vp = p * v 

		spriteShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);	//设置vp矩阵

		//-------------------Circle-----------------------------------
		auto& circleShader = ShaderLibrary::Get("Circle");	//Circle着色器
		circleShader->Bind();								//绑定着色器

		circleShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);	//设置vp矩阵

		//-------------------Line-----------------------------------
		auto& lineShader = ShaderLibrary::Get("Line");	//Line着色器
		lineShader->Bind();								//绑定着色器

		lineShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);	//设置vp矩阵
	}

	void Renderer3D::DrawSprite(const Transform& transform, SpriteRenderer& spriteRenderer, int objectID)
	{
		Sprite& sprite = spriteRenderer.GetSprite();

		auto& spriteShader = ShaderLibrary::Get("Sprite");	//Sprite着色器

		spriteShader->Bind();			//绑定着色器

		sprite.GetTexture()->Bind();	//绑定纹理

		//设置uniform数据
		spriteShader->SetInt("u_TextureExist", sprite.GetTextureExist());
		spriteShader->SetInt("u_Texture", 0);

		sprite.GetVertexBufferData().clear();	//清空上一次顶点缓冲区数据
		//设置顶点数据
		for (int i = 0; i < 4; i++) {
			Vertex vertex = sprite.GetVertices()[i];

			vertex.Position = transform.GetTransform() * glm::vec4(vertex.Position, 1.0f);
			vertex.Color = spriteRenderer.GetColor();
			vertex.ObjectID = objectID;

			sprite.GetVertexBufferData().push_back(vertex);	//添加顶点缓冲区数据
		}

		//SpriteRenderer组件已启用
		if (spriteRenderer.GetEnable()) {
			Processing<Sprite, Vertex>(sprite, RenderCommandType::Triangle);	//渲染Sprite
		}

		s_Data.Stats.TriangleCount += 2;	//三角形个数
		s_Data.Stats.VertexCount += 4;		//累计顶点个数
		s_Data.Stats.IndexCount += 6;		//累计顶点索引个数
	}

	void Renderer3D::DrawCircle(const Transform& transform, CircleRenderer& circleRenderer, int objectID)
	{
		Circle& circle = circleRenderer.GetCircle();

		auto& circleShader = ShaderLibrary::Get("Circle");	//Circle着色器
		circleShader->Bind();								//绑定着色器

		circle.GetVertexBufferData().clear();	//清空上一次顶点缓冲区数据
		//设置顶点数据
		for (int i = 0; i < 4; i++) {
			CircleVertex vertex = circle.GetVertices()[i];

			vertex.WorldPosition = transform.GetTransform() * glm::vec4(vertex.WorldPosition, 1.0f);
			vertex.Color = circle.GetColor();
			vertex.Thickness = circle.GetThickness();
			vertex.Fade = circle.GetFade();
			vertex.ObjectID = objectID;

			circle.GetVertexBufferData().push_back(vertex);	//添加顶点缓冲区数据
		}

		//CircleRenderer组件已启用
		if (circleRenderer.GetEnable()) {
			Processing<Circle, CircleVertex>(circle, RenderCommandType::Triangle);	//渲染Circle
		}

		s_Data.Stats.TriangleCount += 2;	//三角形个数
		s_Data.Stats.VertexCount += 4;		//累计顶点个数
		s_Data.Stats.IndexCount += 6;		//累计顶点索引个数
	}

	void Renderer3D::DrawCircle(const glm::mat4& transform, Circle& circle, int objectID)
	{
		auto& circleShader = ShaderLibrary::Get("Circle");	//Circle着色器
		circleShader->Bind();								//绑定着色器

		circle.GetVertexBufferData().clear();	//清空上一次顶点缓冲区数据
		//设置顶点数据
		for (int i = 0; i < 4; i++) {
			CircleVertex vertex = circle.GetVertices()[i];

			vertex.WorldPosition = transform * glm::vec4(vertex.WorldPosition, 1.0f);
			vertex.Color = circle.GetColor();
			vertex.Thickness = circle.GetThickness();
			vertex.Fade = circle.GetFade();
			vertex.ObjectID = objectID;

			circle.GetVertexBufferData().push_back(vertex);	//添加顶点缓冲区数据
		}

		Processing<Circle, CircleVertex>(circle, RenderCommandType::Triangle);	//渲染Circle

		s_Data.Stats.TriangleCount += 2;	//三角形个数
		s_Data.Stats.VertexCount += 4;		//累计顶点个数
		s_Data.Stats.IndexCount += 6;		//累计顶点索引个数
	}

	void Renderer3D::DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int objectID)
	{
		Line line(p0, p1, color, objectID);	//创建直线

		DrawLine(line);	//绘制直线
	}

	void Renderer3D::DrawLine(Line& line)
	{
		auto& lineShader = ShaderLibrary::Get("Line");	//Line着色器
		lineShader->Bind();								//绑定着色器

		RenderCommand::SetLineWidth(line.GetWidth());					//设置线宽
		Processing<Line, LineVertex>(line, RenderCommandType::Line);	//渲染Line
	}

	void Renderer3D::DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int objectID)
	{
		glm::vec3 p0 = { position.x - size.x * 0.5f, position.y - size.y * 0.5f, position.z };	//左下
		glm::vec3 p1 = { position.x + size.x * 0.5f, position.y - size.y * 0.5f, position.z };	//右下
		glm::vec3 p2 = { position.x + size.x * 0.5f, position.y + size.y * 0.5f, position.z };	//右上
		glm::vec3 p3 = { position.x - size.x * 0.5f, position.y + size.y * 0.5f, position.z };	//左上

		DrawLine(p0, p1, color, objectID);
		DrawLine(p1, p2, color, objectID);
		DrawLine(p2, p3, color, objectID);
		DrawLine(p3, p0, color, objectID);
	}

	void Renderer3D::DrawRect(const glm::mat4& transform, Rectangle& rectangle, int objectID)
	{
		auto& lineShader = ShaderLibrary::Get("Line");	//Line着色器
		lineShader->Bind();								//绑定着色器

		rectangle.GetVertexBufferData().clear();	//清空上一次顶点缓冲区数据
		//设置顶点数据
		for (int i = 0; i < 4; i++) {
			LineVertex vertex = rectangle.GetVertices()[i];

			vertex.Position = transform * glm::vec4(vertex.Position, 1.0f);
			vertex.Color = rectangle.GetColor();

			vertex.ObjectID = objectID;

			rectangle.GetVertexBufferData().push_back(vertex);	//添加顶点缓冲区数据
		}

		Processing<Rectangle, LineVertex>(rectangle, RenderCommandType::Rectangle);	//渲染Rectangle
	}

	void Renderer3D::BeginScene(Environment& environment, const Camera& camera, Transform& transform, std::vector<Object>& lightObjects)
	{
		s_Data.CurrentLightCount = lightObjects.size();	//当前光源数量

		auto& standardShader = ShaderLibrary::Get("Standard");		//标准着色器
		standardShader->Bind();	//绑定标准着色器

		glm::mat4& viewProjectionMatrix = camera.GetProjection() * glm::inverse(transform.GetTransform());	//计算VP矩阵 vp = p * v 

		standardShader->SetMat4("u_ViewProjectionMatrix", viewProjectionMatrix);	//设置vp矩阵
		standardShader->SetFloat3("u_CameraPos", transform.GetPosition());			//设置相机位置
		standardShader->SetInt("u_LightCount", s_Data.CurrentLightCount);			//光源数量

		//遍历Light对象列表
		for (int i = 0; i < s_Data.CurrentLightCount; i++) {
			Transform& lightTransform = lightObjects[i].GetComponent<Transform>();	//光源Transform
			Light& light = lightObjects[i].GetComponent<Light>();					//光源Light

			//设置Light Uniform数据
			light.SetShaderData(lightTransform.GetPosition(), -lightTransform.GetForwardDirection(), standardShader, i);
		}

		//-------------------Skybox-----------------------------------
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

		//-------------------Skybox-----------------------------------
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

	void Renderer3D::EndScene(Environment& environment, const Camera& camera, Transform& transform)
	{
		//启用天空盒
		if (environment.GetSkyboxEnable()) {
			//清屏标志为Skybox
			if (camera.GetClearFlag() == Camera::ClearFlag::Skybox) {
				glDepthFunc(GL_LEQUAL);	//TODO:转移到RenderCommand
				Skybox& skybox = environment.GetSkybox();	//天空盒
				auto& skyboxShader = skybox.GetShader();	//Skybox着色器
				skyboxShader->Bind();						//绑定Skybox着色器
				skybox.GetCubemap()->Bind();				//绑定Cubemap纹理

				glm::mat4& rotationMatrix = glm::rotate(glm::mat4(1.0f), skybox.GetRotation(), { 0, 1, 0 });	//旋转矩阵（绕y轴）
				glm::mat4& viewMatrix = glm::mat4(glm::mat3(glm::inverse(transform.GetTransform())));			//移除view矩阵位移部分 使相机永远处于天空盒中心

				skyboxShader->SetMat4("u_RotationMatrix", rotationMatrix);				//设置旋转矩阵
				skyboxShader->SetMat4("u_ProjectionMatrix", camera.GetProjection());	//设置projection矩阵
				skyboxShader->SetMat4("u_ViewMatrix", viewMatrix);						//设置view矩阵
				skyboxShader->SetInt("u_Cubemap", 0);									//设置Cubemap 0号槽位
				skyboxShader->SetFloat3("u_TintColor", skybox.GetTintColor());			//设置Skybox色调
				skyboxShader->SetFloat("u_Expose", skybox.GetExpose());					//设置Skybox曝光度

				RenderCommand::DrawIndexed(skybox.GetVertexArray());
				glDepthFunc(GL_LESS);	//TODO:转移到RenderCommand
			}
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
				Processing<SubMesh, Vertex>(subMesh, RenderCommandType::Triangle);	//渲染子网格
			}
		}

		s_Data.Stats.TriangleCount += mesh.GetVertexIndexCount() / 3;	//三角形个数
		s_Data.Stats.VertexCount += mesh.GetVertexCount();				//累计顶点个数（实际顶点个数）
		s_Data.Stats.IndexCount += mesh.GetVertexIndexCount();			//累计顶点索引个数
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