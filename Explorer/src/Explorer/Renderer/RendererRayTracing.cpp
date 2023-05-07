#include "exppch.h"
#include "RendererRayTracing.h"

#include "Explorer/Utils/Random.h"

#include "RenderCommand.h"
#include "Shader.h"
#include "VertexArray.h"

#include "Explorer/Scene/Object.h"

namespace RayTracing
{
	/// <summary>
	/// 渲染器数据
	/// </summary>
	struct RendererData
	{
		std::shared_ptr<Explorer::Shader> Shader;
		std::shared_ptr<Explorer::VertexArray> FinalRectVertexArray;		//渲染结果矩形区域VAO
		std::shared_ptr<Explorer::VertexBuffer> FinalRectVertexBuffer;	//渲染结果矩形区域VBO
	};

	static RendererData s_Data;	//渲染数据

	void Renderer::Init()
	{
		s_Data.Shader = std::make_shared<Explorer::Shader>("assets/shaders/RayTracingShader");	//光线追踪着色器

		float finalRectVertexPositions[12] =
		{
			-1.0f, -1.0f, 0.0f,	//左下 0
			 1.0f, -1.0f, 0.0f,	//右下 1
			 1.0f,  1.0f, 0.0f,	//右上 2
			-1.0f,  1.0f, 0.0f	//左上 3
		};

		uint32_t finalRectVertexIndices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		s_Data.FinalRectVertexArray = std::make_shared<Explorer::VertexArray>();				//创建顶点数组VAO
		s_Data.FinalRectVertexBuffer = std::make_shared<Explorer::VertexBuffer>(finalRectVertexPositions, 12 * sizeof(float));	//创建顶点缓冲VBO

		//设置顶点缓冲区布局
		s_Data.FinalRectVertexBuffer->SetLayout({
			{Explorer::ShaderDataType::Float3, "a_Position"},	//位置
		});
		s_Data.FinalRectVertexArray->AddVertexBuffer(s_Data.FinalRectVertexBuffer);	//添加VBO到VAO

		std::shared_ptr<Explorer::IndexBuffer> indexBuffer = std::make_shared<Explorer::IndexBuffer>(finalRectVertexIndices, 6);	//创建索引缓冲EBO
		s_Data.FinalRectVertexArray->SetIndexBuffer(indexBuffer);												//设置EBO到VAO
	}

	void Renderer::Shutdown()
	{

	}

	void Renderer::BeginScene(const Explorer::EditorCamera& camera)
	{
		s_Data.Shader->Bind();

		s_Data.Shader->SetFloat3("u_Camera.Position", camera.GetPosition());
		s_Data.Shader->SetFloat2("u_Camera.ViewportSize", camera.GetViewportSize());
		s_Data.Shader->SetMat4("u_Camera.ProjectionMatrix", camera.GetProjectionMatrix());
		s_Data.Shader->SetMat4("u_Camera.ViewMatrix", camera.GetViewMatrix());
	}

	void Renderer::BeginScene(const Explorer::Camera& camera, Explorer::Transform& transform, const glm::vec2& viewportSize)
	{
		s_Data.Shader->Bind();

		s_Data.Shader->SetFloat3("u_Camera.Position", transform.GetPosition());
		s_Data.Shader->SetFloat2("u_Camera.ViewportSize", viewportSize);
		s_Data.Shader->SetMat4("u_Camera.ProjectionMatrix", camera.GetProjection());
		s_Data.Shader->SetMat4("u_Camera.ViewMatrix", glm::inverse(transform.GetTransform()));
	}

	void Renderer::Render(const Scene& scene)
	{
		s_Data.Shader->Bind();

		//设置场景球体uniform数据
		s_Data.Shader->SetInt("u_Scene.ObjectCount", scene.Spheres.size());
		s_Data.Shader->SetInt("u_Scene.MaterialCount", scene.Materials.size());

		for (int i = 0; i < scene.Spheres.size(); i++) {
			Explorer::Object sphere = scene.SphereObjects[i];
			Explorer::Transform& transform = sphere.GetComponent<Explorer::Transform>();
			glm::vec3 position = transform.GetTransform() * glm::vec4(scene.Spheres[i].Position, 1.0f);
			float radius = transform.GetScale().x * scene.Spheres[i].Radius;

			s_Data.Shader->SetFloat3("u_Scene.Spheres[" + std::to_string(i) + "].Center", position);
			s_Data.Shader->SetFloat("u_Scene.Spheres[" + std::to_string(i) + "].Radius", radius);
			s_Data.Shader->SetInt("u_Scene.Spheres[" + std::to_string(i) + "].MaterialIndex", scene.Spheres[i].MaterialIndex);
		}

		for (int i = 0; i < scene.Materials.size(); i++) {
			s_Data.Shader->SetFloat3("u_Scene.Materials[" + std::to_string(i) + "].Albedo", scene.Materials[i].Albedo);
			s_Data.Shader->SetFloat("u_Scene.Materials[" + std::to_string(i) + "].Roughness", scene.Materials[i].Roughness);
		}

		Explorer::RenderCommand::DrawIndexed(s_Data.FinalRectVertexArray);	//绘制渲染结果矩形
	}
}