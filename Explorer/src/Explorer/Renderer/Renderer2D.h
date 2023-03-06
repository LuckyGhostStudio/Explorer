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
		/// ��ʼ����Ⱦ��
		/// </summary>
		static void Init();

		static void Shutdown();

		/// <summary>
		/// ��ʼ��Ⱦ���������ó�������
		/// </summary>
		/// <param name="camera">���</param>
		/// <param name="transform">�任</param>
		static void BeginScene(const Camera& camera, Transform& transform);

		/// <summary>
		/// ������Ⱦ����
		/// </summary>
		static void EndScene();

		/// <summary>
		/// ˢ��
		/// </summary>
		static void Flush();

		/// <summary>
		/// ���Ƴ�����
		/// </summary>
		/// <param name="position">λ��</param>
		/// <param name="rotation">��ת</param>
		/// <param name="scale">��С</param>
		/// <param name="color">��ɫ</param>
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color);

		/// <summary>
		/// ���Ƴ�����
		/// </summary>
		/// <param name="transform">�任</param>
		/// <param name="color">��ɫ</param>
		static void DrawQuad(Transform& transform, const glm::vec4& color);

		/// <summary>
		/// ���Ƴ�����
		/// </summary>
		/// <param name="position">λ��</param>
		/// <param name="rotation">��ת</param>
		/// <param name="scale">��С</param>
		/// <param name="color">��ɫ</param>
		/// <param name="texture">����</param>
		/// <param name="tilingFactor">�����ظ�����</param>
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tilingFactor = glm::vec2(1.0f));

		/// <summary>
		/// ���Ƴ�����
		/// </summary>
		/// <param name="position">λ��</param>
		/// <param name="rotation">��ת</param>
		/// <param name="scale">��С</param>
		/// <param name="color">��ɫ</param>
		/// <param name="texture">����</param>
		/// <param name="tilingFactor">�����ظ�����</param>
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tilingFactor = glm::vec2(1.0f));
	
		static void DrawQuad(const glm::mat4& transform, const glm::vec4& color);
		static void DrawQuad(const glm::mat4& transform, const std::shared_ptr<Texture2D>& texture, const glm::vec2& tilingFactor = glm::vec2(1.0f), const glm::vec4& color = glm::vec4(1.0f));

		/// <summary>
		/// ���Ƴ�����
		/// </summary>
		/// <param name="position">λ��</param>
		/// <param name="rotation">��ת</param>
		/// <param name="scale">��С</param>
		/// <param name="color">��ɫ</param>
		/// <param name="subTexture">������</param>
		/// <param name="tilingFactor">�����ظ�����</param>
		static void DrawQuad(const glm::vec2& position, float rotation, const glm::vec2& scale, const glm::vec4& color, const std::shared_ptr<SubTexture2D>& subTexture, const glm::vec2& tilingFactor = glm::vec2(1.0f));

		/// <summary>
		/// ���Ƴ�����
		/// </summary>
		/// <param name="position">λ��</param>
		/// <param name="rotation">��ת</param>
		/// <param name="scale">��С</param>
		/// <param name="color">��ɫ</param>
		/// <param name="subTexture">������</param>
		/// <param name="tilingFactor">�����ظ�����</param>
		static void DrawQuad(const glm::vec3& position, float rotation, const glm::vec3& scale, const glm::vec4& color, const std::shared_ptr<SubTexture2D>& subTexture, const glm::vec2& tilingFactor = glm::vec2(1.0f));


		/// <summary>
		/// ͳ������
		/// </summary>
		struct Statistics
		{
			uint32_t DrawCalls = 0;	//���Ƶ��ô���
			uint32_t QuadCount = 0;	//�ı��θ���

			/// <summary>
			/// �����ܶ������
			/// </summary>
			/// <returns></returns>
			uint32_t GetTotalVertexCount() { return QuadCount * 4; }

			/// <summary>
			/// ��������������
			/// </summary>
			/// <returns></returns>
			uint32_t GetTotalIndexCount() { return QuadCount * 6; }
		};

		static Statistics GetStats();

		/// <summary>
		/// ����ͳ������
		/// </summary>
		static void ResetStats();
	private:
		/// <summary>
		/// ��ʼ��һ����Ⱦ
		/// </summary>
		static void FlushAndReset();
	};
}