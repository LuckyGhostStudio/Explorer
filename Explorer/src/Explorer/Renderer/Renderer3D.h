#pragma once

#include "Explorer/Components/Transform.h"
#include "Explorer/Components/Camera.h"
#include "Explorer/Components/Light.h"
#include "Explorer/Components/Components.h"
#include "Explorer/Renderer/EditorCamera.h"

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
		/// <param name="camera">场景相机</param>
		static void BeginScene(const Camera& camera, const Transform& transform);

		/// <summary>
		/// 开始渲染场景
		/// </summary>
		/// <param name="camera">编辑器相机</param>
		/// <param name="lightObjects">场景所有光源对象</param>
		static void BeginScene(const EditorCamera& camera, std::vector<Object>& lightObjects);

		/// <summary>
		/// 结束渲染场景
		/// </summary>
		static void EndScene();

		/// <summary>
		/// 开始网格处理：设置网格渲染初始参数
		/// </summary>
		static void StartMeshProcessing();

		/// <summary>
		/// 开始下一个网格处理：结束上一个网格渲染 开始下一个
		/// </summary>
		static void NextMeshProcessing();

		/// <summary>
		/// 网格渲染处理过程
		/// </summary>
		static void MeshProcessing();

		static void DrawMesh(const Transform& transform, int objectID = -1);

		/// <summary>
		/// 统计数据
		/// </summary>
		struct Statistics
		{
			uint32_t DrawCalls = 0;		//绘制调用次数
			uint32_t TriangleCount = 0;	//三角形个数
			uint32_t VertexCount = 0;	//顶点个数
			uint32_t IndexCount = 0;	//索引个数
		};

		static Statistics GetStats();

		/// <summary>
		/// 重置统计数据
		/// </summary>
		static void ResetStats();
	};
}