#pragma once

#include "Camera.h"

namespace Explorer
{
	class Renderer2D
	{
	public:
		/// <summary>
		/// ��ʼ����Ⱦ��
		/// </summary>
		static void Init();

		static void Shutdown();

		/// <summary>
		/// ��ʼ��Ⱦ���������ó�������
		/// </summary>
		/// <param name="camera">���</param>
		static void BeginScene(const Camera& camera);

		/// <summary>
		/// ������Ⱦ����
		/// </summary>
		static void EndScene();

		/// <summary>
		/// ���Ƴ�����
		/// </summary>
		/// <param name="position">λ��</param>
		/// <param name="size">��С</param>
		/// <param name="color">��ɫ</param>
		static void DrawQuad(const glm::vec2 position, const glm::vec2 size, const glm::vec4 color);

		/// <summary>
		/// ���Ƴ�����
		/// </summary>
		/// <param name="position">λ��</param>
		/// <param name="size">��С</param>
		/// <param name="color">��ɫ</param>
		static void DrawQuad(const glm::vec3 position, const glm::vec2 size, const glm::vec4 color);
	};
}