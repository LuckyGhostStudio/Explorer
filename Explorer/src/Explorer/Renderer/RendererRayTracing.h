#pragma once

#include "Texture.h"

#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// ����׷����Ⱦ��
	/// </summary>
	class RendererRayTracing
	{
	private:
		std::shared_ptr<Texture2D> m_FinalImage;	//��Ⱦ���ͼ��
		uint32_t* m_ImageData = nullptr;			//ͼ������
	public:
		RendererRayTracing() = default;

		/// <summary>
		/// ����ͼ���С
		/// </summary>
		/// <param name="width">��</param>
		/// <param name="height">��</param>
		void OnResize(uint32_t width, uint32_t height);

		/// <summary>
		/// ��Ⱦͼ����Ⱦÿ������
		/// </summary>
		void Render();

		std::shared_ptr<Texture2D> GetFinalImage() const { return m_FinalImage; }
	private:
		/// <summary>
		/// ����ÿ������
		/// </summary>
		/// <param name="coord">��������</param>
		/// <returns>�����ɫ</returns>
		glm::vec4 PerPixel(glm::vec2 coord);
	};
}