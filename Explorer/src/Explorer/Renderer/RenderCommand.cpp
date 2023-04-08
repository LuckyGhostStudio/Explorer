#include "exppch.h"
#include "RenderCommand.h"

#include <glad/glad.h>

namespace Explorer
{
	void RenderCommand::Init()
	{
		glEnable(GL_BLEND);									//启用混合
		glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);	//最终颜色 = src * alpha + des * (1 - alpha)

		glEnable(GL_DEPTH_TEST);							//启用深度测试
	}

	void RenderCommand::SetViewport(uint32_t x, uint32_t y, uint32_t width, uint32_t height)
	{
		glViewport(x, y, width, height);	//设置视口大小
	}

	void RenderCommand::SetClearColor(const glm::vec4& color)
	{
		glClearColor(color.r, color.g, color.b, color.a);	//设置清屏颜色
	}
	
	void RenderCommand::Clear()
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);	//清除 颜色缓冲区|深度缓冲区
	}

	void RenderCommand::SetDepthMask(bool enable)
	{
		glDepthMask(enable);
	}
	
	void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray, uint32_t indexCount)
	{
		vertexArray->Bind();

		indexCount = 0;

		uint32_t count = indexCount ? vertexArray->GetIndexBuffer()->GetCount() : indexCount;
		glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, nullptr);	//顶点数组索引绘制三角形
		glBindTexture(GL_TEXTURE_2D, 0);	//清空纹理槽
	}

	void RenderCommand::DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray)
	{
		vertexArray->Bind();

		glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, nullptr);	//顶点数组索引绘制三角形
		glBindTexture(GL_TEXTURE_2D, 0);	//清空纹理槽
	}
}