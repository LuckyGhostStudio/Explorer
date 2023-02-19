#pragma once

#include "VertexArray.h"
#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// 渲染命令
	/// </summary>
	class RenderCommand
	{
	public:
		/// <summary>
		/// 设置清屏颜色
		/// </summary>
		/// <param name="color">清屏颜色</param>
		static void SetClearColor(const glm::vec4& color);

		/// <summary>
		/// 清屏
		/// </summary>
		static void Clear();

		/// <summary>
		/// 绘制索引缓冲区
		/// </summary>
		/// <param name="vertexArray">待绘制的顶点数组</param>
		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
	};
}