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
		//glEnable(GL_LINE_SMOOTH);							//启用直线反走样
		glEnable(GL_STENCIL_TEST);							//启用模板测试
		glStencilOp(GL_KEEP, GL_KEEP, GL_REPLACE);			//在深度和模板测试都通过时 使用glStencilFunc指定的参考值
		//glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);			//线框模式
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
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);	//清除 颜色缓冲区|深度缓冲区|模板缓冲区
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

	void RenderCommand::DrawLines(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();

		glDrawArrays(GL_LINES, 0, vertexCount);	//顶点数组绘制直线
	}

	void RenderCommand::DrawLoopLine(const std::shared_ptr<VertexArray>& vertexArray, uint32_t vertexCount)
	{
		vertexArray->Bind();

		glDrawArrays(GL_LINE_LOOP, 0, vertexCount);	//顶点数组绘制循环直线
	}
	
	void RenderCommand::SetLineWidth(float width)
	{
		glLineWidth(width);	//设置线宽
	}
}