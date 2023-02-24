#pragma once

#include "Camera.h"
#include "Texture.h"

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
		static void BeginScene(const Camera& camera);

		/// <summary>
		/// 结束渲染场景
		/// </summary>
		static void EndScene();

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
		/// <param name="position">位置</param>
		/// <param name="rotation">旋转</param>
		/// <param name="scale">大小</param>
		/// <param name="color">颜色</param>
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color);

		/// <summary>
		/// 绘制长方形
		/// </summary>
		/// <param name="position">位置</param>
		/// <param name="rotation">旋转</param>
		/// <param name="scale">大小</param>
		/// <param name="color">颜色</param>
		/// <param name="texture">纹理</param>
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture);

		/// <summary>
		/// 绘制长方形
		/// </summary>
		/// <param name="position">位置</param>
		/// <param name="rotation">旋转</param>
		/// <param name="scale">大小</param>
		/// <param name="color">颜色</param>
		/// <param name="texture">纹理</param>
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture);
	};
}