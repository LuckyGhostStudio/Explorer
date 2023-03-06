#pragma once

#include "Camera.h"
#include "Texture.h"
#include "SubTexture2D.h"
#include "Explorer/Scene/Components.h"

namespace Explorer
{
	class Renderer2D
	{
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
		/// <param name="transform">变换</param>
		static void BeginScene(const Camera& camera, Transform& transform);

		/// <summary>
		/// 结束渲染场景
		/// </summary>
		static void EndScene();

		/// <summary>
		/// 刷新
		/// </summary>
		static void Flush();

		/// <summary>
		/// 绘制长方形
		/// </summary>
		/// <param name="position">位置</param>
		/// <param name="rotation">旋转</param>
		/// <param name="scale">大小</param>
		/// <param name="color">颜色</param>
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color);

		/// <summary>
		/// 绘制长方形
		/// </summary>
		/// <param name="transform">变换</param>
		/// <param name="color">颜色</param>
		static void DrawQuad(Transform& transform, const glm::vec4& color);

		/// <summary>
		/// 绘制长方形
		/// </summary>
		/// <param name="position">位置</param>
		/// <param name="rotation">旋转</param>
		/// <param name="scale">大小</param>
		/// <param name="color">颜色</param>
		/// <param name="texture">纹理</param>
		/// <param name="tilingFactor">纹理重复因子</param>
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tilingFactor = glm::vec2(1.0f));

		/// <summary>
		/// 绘制长方形
		/// </summary>
		/// <param name="position">位置</param>
		/// <param name="rotation">旋转</param>
		/// <param name="scale">大小</param>
		/// <param name="color">颜色</param>
		/// <param name="texture">纹理</param>
		/// <param name="tilingFactor">纹理重复因子</param>
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tilingFactor = glm::vec2(1.0f));
	
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tilingFactor = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f));

		/// <summary>
		/// 绘制长方形
		/// </summary>
		/// <param name="position">位置</param>
		/// <param name="rotation">旋转</param>
		/// <param name="scale">大小</param>
		/// <param name="color">颜色</param>
		/// <param name="subTexture">子纹理</param>
		/// <param name="tilingFactor">纹理重复因子</param>
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color, const std::shared_ptr<SubTexture2D>& subTexture, const glm::vec2& tilingFactor = glm::vec2(1.0f));

		/// <summary>
		/// 绘制长方形
		/// </summary>
		/// <param name="position">位置</param>
		/// <param name="rotation">旋转</param>
		/// <param name="scale">大小</param>
		/// <param name="color">颜色</param>
		/// <param name="subTexture">子纹理</param>
		/// <param name="tilingFactor">纹理重复因子</param>
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color, const std::shared_ptr<SubTexture2D>& subTexture, const glm::vec2& tilingFactor = glm::vec2(1.0f));


		/// <summary>
		/// 统计数据
		/// </summary>
		struct Statistics
		{
			uint32_t DrawCalls = 0;	//绘制调用次数
			uint32_t QuadCount = 0;	//四边形个数

			/// <summary>
			/// 返回总顶点个数
			/// </summary>
			/// <returns></returns>
			uint32_t GetTotalVertexCount() { return QuadCount * 4; }

			/// <summary>
			/// 返回总索引个数
			/// </summary>
			/// <returns></returns>
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static Statistics GetStats();

		/// <summary>
		/// 重置统计数据
		/// </summary>
		static void ResetStats();
	private:
		/// <summary>
		/// 开始新一批渲染
		/// </summary>
		static void FlushAndReset();
	};
}