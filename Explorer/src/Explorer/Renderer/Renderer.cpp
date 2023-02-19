#include "exppch.h"
#include "Renderer.h"

namespace Explorer
{
	void Renderer::BeginScene()
	{

	}
	
	void Renderer::EndScene()
	{

	}
	
	void Renderer::Submit(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();						//绑定顶点数组
		RenderCommand::DrawIndexed(vertexArray);	//绘制
	}
}