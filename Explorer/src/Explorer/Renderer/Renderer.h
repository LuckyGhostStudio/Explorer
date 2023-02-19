#pragma once

#include "RenderCommand.h"

namespace Explorer
{
	/// <summary>
	/// 渲染器
	/// </summary>
	class Renderer
	{
	public:
		/// <summary>
		/// 开始渲染场景：设置场景参数（着色器，环境光，灯光，变换矩阵，相机...）
		/// </summary>
		static void BeginScene();

		/// <summary>
		/// 结束渲染场景
		/// </summary>
		static void EndScene();

		/// <summary>
		/// 提交渲染命令
		/// </summary>
		/// <param name="vertexArray">待渲染顶点数组</param>
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
	};
}