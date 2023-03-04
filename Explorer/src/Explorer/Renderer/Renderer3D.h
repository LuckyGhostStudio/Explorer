#pragma once

#include "Camera.h"
#include "Explorer/Scene/Components.h"

namespace Explorer
{
	/// <summary>
	/// 3D渲染器
	/// </summary>
	class Renderer3D
	{
	private:
		/// <summary>
		/// 渲染器类型
		/// </summary>
		enum class RendererType
		{
			Rasterization = 0,	//光栅化
			Raytracing = 1		//光线追踪
		};

		static RendererType m_Type;	//渲染器类型（0 光栅化渲染器 1 光线追踪渲染器）
	public:
		/// <summary>
		/// 初始化渲染器
		/// </summary>
		static void Init();

		static void Shutdown();

		/// <summary>
		/// 开始渲染场景：设置场景参数
		/// </summary>
		/// <param name="camera">相机</param>
		static void BeginScene(const Camera& camera, const Transform& transform);

		/// <summary>
		/// 结束渲染场景
		/// </summary>
		static void EndScene();

		static void DrawMesh(const Transform& transform);
	};
}