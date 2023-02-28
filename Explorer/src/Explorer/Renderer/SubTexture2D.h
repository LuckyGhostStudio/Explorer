#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Explorer
{
	/// <summary>
	/// 子纹理
	/// </summary>
	class SubTexture2D
	{
	private:
		std::shared_ptr<Texture2D> m_Texture;	//纹理
		glm::vec2 m_TexCoords[4];				//子纹理坐标
	public:
		/// <summary>
		/// 子纹理
		/// </summary>
		/// <param name="texture">纹理</param>
		/// <param name="min">左下角纹理坐标</param>
		/// <param name="max">右上角纹理坐标</param>
		SubTexture2D(const std::shared_ptr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		/// <summary>
		/// 返回纹理
		/// </summary>
		/// <returns>纹理</returns>
		const std::shared_ptr<Texture2D> GetTexture() const { return m_Texture; }

		/// <summary>
		/// 返回子纹理坐标
		/// </summary>
		/// <returns>子纹理坐标</returns>
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }

		/// <summary>
		/// 创建子纹理
		/// </summary>
		/// <param name="texture">纹理</param>
		/// <param name="coords">子纹理坐标</param>
		/// <param name="cellSize">子纹理单元格大小（像素）</param>
		/// <param name="spriteSize">子纹理大小（单位）</param>
		/// <returns></returns>
		static std::shared_ptr<SubTexture2D> CreateFromCoords(const std::shared_ptr<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1, 1 });
	};
}