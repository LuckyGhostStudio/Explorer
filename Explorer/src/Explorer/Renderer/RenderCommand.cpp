#include "exppch.h"
#include "RenderCommand.h"

#include <glad/glad.h>

namespace Explorer
{
	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);	//����������ɫ
	}
	
	void RenderCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//��� ��ɫ������|��Ȼ�����
	}
	
	void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);	//����������������������
	}
}