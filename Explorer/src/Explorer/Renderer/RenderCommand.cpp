#include "exppch.h"
#include "RenderCommand.h"

#include <glad/glad.h>

namespace Explorer
{
	void RenderCommand::Init()
	{
		glEnable(GL_BLEND);									//启用混合
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//最终颜色 = src * alpha + des * (1 - alpha)
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);	//设置清屏颜色
	}
	
	void RenderCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清除 颜色缓冲区|深度缓冲区
	}
	
	void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);	//顶点数组索引绘制三角形
	}
}