#include "exppch.h"
#include "Renderer.h"

namespace Explorer
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();		//场景数据

	void Renderer::BeginScene(Camera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();	//设置场景vp矩阵
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjectionMatrix", m_SceneData->ViewProjectionMatrix);	//设置vp Uniform变量

		vertexArray->Bind();						//绑定顶点数组
		RenderCommand::DrawIndexed(vertexArray);	//绘制
	}
}