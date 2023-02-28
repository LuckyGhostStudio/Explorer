#pragma once

#include <glm/glm.hpp>

#include "Texture.h"

namespace Explorer
{
	/// <summary>
	/// ������
	/// </summary>
	class SubTexture2D
	{
	private:
		std::shared_ptr<Texture2D> m_Texture;	//����
		glm::vec2 m_TexCoords[4];				//����������
	public:
		/// <summary>
		/// ������
		/// </summary>
		/// <param name="texture">����</param>
		/// <param name="min">���½���������</param>
		/// <param name="max">���Ͻ���������</param>
		SubTexture2D(const std::shared_ptr<Texture2D>& texture, const glm::vec2& min, const glm::vec2& max);

		/// <summary>
		/// ��������
		/// </summary>
		/// <returns>����</returns>
		const std::shared_ptr<Texture2D> GetTexture() const { return m_Texture; }

		/// <summary>
		/// ��������������
		/// </summary>
		/// <returns>����������</returns>
		const glm::vec2* GetTexCoords() const { return m_TexCoords; }

		/// <summary>
		/// ����������
		/// </summary>
		/// <param name="texture">����</param>
		/// <param name="coords">����������</param>
		/// <param name="cellSize">������Ԫ���С�����أ�</param>
		/// <param name="spriteSize">�������С����λ��</param>
		/// <returns></returns>
		static std::shared_ptr<SubTexture2D> CreateFromCoords(const std::shared_ptr<Texture2D>& texture, const glm::vec2& coords, const glm::vec2& cellSize, const glm::vec2& spriteSize = { 1, 1 });
	};
}