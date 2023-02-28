#include "exppch.h"
#include "SubTexture2D.h"

namespace Explorer
{
	SubTexture2D::SubTexture2D(const std::shared_ptr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max) :m_Texture(texture)
	{
		m_TexCoords[0] = { min.x, min.y };	//左下
		m_TexCoords[1] = { max.x, min.y };	//右下
		m_TexCoords[2] = { max.x, max.y };	//右上
		m_TexCoords[3] = { min.x, max.y };	//左上
	}
	
	std::shared_ptr<SubTexture2D> SubTexture2D::CreateFromCoords(const std::shared_ptr<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize)
	{
		//左下角
		glm::vec2 min = { (coords.x * cellSize.x) / texture->GetWidth(), (coords.y * cellSize.y) / texture->GetHeight() };
		//右上角
		glm::vec2 max = { ((coords.x + spriteSize.x) * cellSize.x) / texture->GetWidth(), ((coords.y + spriteSize.y) * cellSize.y) / texture->GetHeight() };
		
		return std::make_shared<SubTexture2D>(texture, min, max);
	}
}