#pragma once

#include "RenderCommand.h"

namespace Explorer
{
	/// <summary>
	/// ��Ⱦ��
	/// </summary>
	class Renderer
	{
	public:
		/// <summary>
		/// ��ʼ��Ⱦ���������ó�����������ɫ���������⣬�ƹ⣬�任�������...��
		/// </summary>
		static void BeginScene();

		/// <summary>
		/// ������Ⱦ����
		/// </summary>
		static void EndScene();

		/// <summary>
		/// �ύ��Ⱦ����
		/// </summary>
		/// <param name="vertexArray">����Ⱦ��������</param>
		static void Submit(const std::shared_ptr<VertexArray>& vertexArray);
	};
}