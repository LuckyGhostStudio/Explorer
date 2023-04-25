#pragma once

#include "Explorer/Components/Transform.h"
#include "Explorer/Components/Camera.h"
#include "Explorer/Components/Light.h"
#include "Explorer/Components/Mesh.h"
#include "Explorer/Components/Material.h"
#include "Explorer/Components/SpriteRenderer.h"
#include "Explorer/Components/CircleRenderer.h"
#include "Explorer/Components/Components.h"

#include "Explorer/Components/Line.h"
#include "Explorer/Components/Rectangle.h"

#include "Explorer/Renderer/EditorCamera.h"
#include "Explorer/Renderer/Environment.h"

#include "Explorer/Scene/Object.h"

namespace Explorer
{
	/// <summary>
	/// 渲染命令类型
	/// </summary>
	enum class RenderCommandType
	{
		Triangle = 0,
		Line = 1,
		Rectangle = 2
	};

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

		static RendererType s_Type;	//渲染器类型（0 光栅化渲染器 1 光线追踪渲染器）
	public:
		/// <summary>
		/// 初始化渲染器
		/// </summary>
		static void Init();

		static void Shutdown();

		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const Camera& camera, Transform& transform);

		static void DrawSprite(const Transform& transform, SpriteRenderer& spriteRenderer, /*Material& material,*/ int objectID = -1);
		static void DrawCircle(const Transform& transform, CircleRenderer& circleRenderer, int objectID = -1);

		static void DrawCircle(const glm::mat4& transform, Circle& circle, int objectID = -1);

		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int objectID = -1);
		static void DrawLine(Line& line);

		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int objectID = -1);
		static void DrawRect(const glm::mat4& transform, Rectangle& rectangle, int objectID = -1);
		
		/// <summary>
		/// 渲染处理
		/// </summary>
		/// <typeparam name="T">渲染对象类型</typeparam>
		/// <typeparam name="V">顶点类型</typeparam>
		/// <param name="t">渲染对象</param>
		/// <param name="type">渲染命令类型</param>
		template<typename T, typename V>
		static void Processing(T& t, RenderCommandType type);

		/// <summary>
		/// 开始渲染场景：设置场景参数
		/// </summary>
		/// <param name="camera">场景相机</param>
		static void BeginScene(Environment& environment, const Camera& camera, Transform& transform, std::vector<Object>& lightObjects);

		/// <summary>
		/// 开始渲染场景
		/// </summary>
		/// <param name="environment">环境设置</param>
		/// <param name="camera">编辑器相机</param>
		/// <param name="lightObjects">场景所有光源对象</param>
		static void BeginScene(Environment& environment, const EditorCamera& camera, std::vector<Object>& lightObjects);

		/// <summary>
		/// 结束渲染场景
		/// </summary>
		static void EndScene(Environment& environment, const EditorCamera& camera);
		static void EndScene(Environment& environment, const Camera& camera, Transform& transform);

		static void EndScene();

		/// <summary>
		/// 子网格处理
		/// </summary>
		/// <param name="subMesh">子网格</param>
		static void SubMeshProcessing(SubMesh& subMesh);

		static void DrawMesh(const Transform& transform, Mesh& mesh, Material& material, int objectID = -1);

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
	
	template<typename T, typename V>
	inline void Renderer3D::Processing(T& t, RenderCommandType type)
	{
		uint32_t dataSize = (uint32_t)sizeof(V) * t.GetVertexBufferData().size();	//计算顶点缓冲区数据大小（字节）

		t.GetVertexBuffer()->SetData(t.GetVertexBufferData().data(), dataSize);	//设置顶点缓冲区数据

		switch (type)
		{
			case RenderCommandType::Triangle:
				RenderCommand::DrawIndexed(t.GetVertexArray());		//绘制调用：绘制三角形
				break;
			case RenderCommandType::Line:
				RenderCommand::DrawLines(t.GetVertexArray(), 2);	//绘制调用：绘制直线
				break;
			case RenderCommandType::Rectangle:
				RenderCommand::DrawLoopLine(t.GetVertexArray(), 4);	//绘制调用：绘制Rectangle
				break;
		}

		s_Data.Stats.DrawCalls++;	//绘制调用次数++
	}
}