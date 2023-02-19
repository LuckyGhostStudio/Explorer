#pragma once

#include "VertexArray.h"
#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// ��Ⱦ����
	/// </summary>
	class RenderCommand
	{
	public:
		/// <summary>
		/// ����������ɫ
		/// </summary>
		/// <param name="color">������ɫ</param>
		static void SetClearColor(const glm::vec4& color);

		/// <summary>
		/// ����
		/// </summary>
		static void Clear();

		/// <summary>
		/// ��������������
		/// </summary>
		/// <param name="vertexArray">�����ƵĶ�������</param>
		static void DrawIndexed(const std::shared_ptr<VertexArray>& vertexArray);
	};
}