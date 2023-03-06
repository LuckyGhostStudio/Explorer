#include "exppch.h"
#include "Renderer.h"
#include "Renderer2D.h"
#include "Renderer3D.h"

namespace Explorer
{
	std::unique_ptr<Renderer::SceneData> Renderer::m_SceneData = std::make_unique<Renderer::SceneData>();		//场景数据

	void Renderer::Init()
	{
		RenderCommand::Init();
		//Renderer2D::Init();
		Renderer3D::Init();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);	//设置视口大小
	}

	void Renderer::BeginScene(Camera& camera)
	{
		//m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();	//设置场景vp矩阵
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjectionMatrix", m_SceneData->ViewProjectionMatrix);	//设置vp Uniform变量
		shader->UploadUniformMat4("u_Transform", transform);									//设置model变换 Uniform变量

		vertexArray->Bind();						//绑定顶点数组
		RenderCommand::DrawIndexed(vertexArray);	//绘制
	}
}