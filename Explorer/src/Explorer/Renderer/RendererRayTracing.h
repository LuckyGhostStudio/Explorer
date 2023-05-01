#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// 光线追踪渲染器
	/// </summary>
	class RendererRayTracing
	{
	private:
		std::shared_ptr<Texture2D> m_FinalImage;	//渲染结果图像
		uint32_t* m_ImageData = nullptr;			//图像数据
	public:
		RendererRayTracing() = default;

		/// <summary>
		/// 重置图像大小
		/// </summary>
		/// <param name="width">宽</param>
		/// <param name="height">高</param>
		void OnResize(uint32_t width, uint32_t height);

		/// <summary>
		/// 渲染图像：渲染每个像素
		/// </summary>
		void Render();

		std::shared_ptr<Texture2D> GetFinalImage() const { return m_FinalImage; }
	private:
		/// <summary>
		/// 处理每个像素
		/// </summary>
		/// <param name="coord">像素坐标</param>
		/// <returns>输出颜色</returns>
		glm::vec4 PerPixel(glm::vec2 coord);
	};
}