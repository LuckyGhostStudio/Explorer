#include "exppch.h"
#include "Renderer.h"

namespace Explorer
{
	Renderer::SceneData* Renderer::m_SceneData = new Renderer::SceneData();		//��������

	void Renderer::BeginScene(Camera& camera)
	{
		m_SceneData->ViewProjectionMatrix = camera.GetViewProjectionMatrix();	//���ó���vp����
	}

	void Renderer::EndScene()
	{

	}

	void Renderer::Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform)
	{
		shader->Bind();
		shader->UploadUniformMat4("u_ViewProjectionMatrix", m_SceneData->ViewProjectionMatrix);	//����vp Uniform����
		shader->UploadUniformMat4("u_Transform", transform);									//����model�任 Uniform����

		vertexArray->Bind();						//�󶨶�������
		RenderCommand::DrawIndexed(vertexArray);	//����
	}
}