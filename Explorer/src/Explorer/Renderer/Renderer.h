#pragma once

#include "RenderCommand.h"
#include "Explorer/Components/Camera.h"
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

		static std::unique_ptr<SceneData> m_SceneData;	//场景数据
	public:
		/// <summary>
		/// 初始化渲染器
		/// </summary>
		static void Init();

		static void Shutdown();

		/// <summary>
		/// 窗口缩放时调用
		/// </summary>
		/// <param name="width">宽</param>
		/// <param name="height">高</param>
		static void OnWindowResize(uint32_t width, uint32_t height);

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
		/// <param name="transform">变换矩阵</param>
		static void Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray, const glm::mat4& transform = glm::mat4(1.0f));
	};
}