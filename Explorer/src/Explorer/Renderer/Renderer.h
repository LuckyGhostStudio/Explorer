#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace Explorer
{
	/// <summary>
	/// 渲染器
	/// </summary>
	class Renderer
	{
	private:
		/// <summary>
		/// 场景数据
		/// </summary>
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;	//vp矩阵
		};

		static SceneData* m_SceneData;	//场景数据
	public:
		/// <summary>
		/// 开始渲染场景：设置场景参数（着色器，环境光，灯光，变换矩阵，相机...）
		/// </summary>
		/// <param name="camera">相机</param>
		static void BeginScene(Camera& camera);

		/// <summary>
		/// 结束渲染场景
		/// </summary>
		static void EndScene();

		/// <summary>
		/// 提交渲染命令
		/// </summary>
		/// <param name="shader">着色器</param>
		/// <param name="vertexArray">待渲染顶点数组</param>
		static void Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray);
	};
}