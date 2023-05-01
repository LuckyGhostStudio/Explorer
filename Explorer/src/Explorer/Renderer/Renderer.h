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
	public:
		/// <summary>
		/// 渲染器类型
		/// </summary>
		enum class Type
		{
			Rasterization = 0,	//光栅化
			Raytracing = 1		//光线追踪
		};

		static Type s_Type;	//渲染器类型（0 光栅化渲染器 1 光线追踪渲染器）
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
	};
}