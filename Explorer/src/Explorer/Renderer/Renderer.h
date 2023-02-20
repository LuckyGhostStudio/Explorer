#pragma once

#include "RenderCommand.h"
#include "Camera.h"
#include "Shader.h"

namespace Explorer
{
	/// <summary>
	/// ��Ⱦ��
	/// </summary>
	class Renderer
	{
	private:
		/// <summary>
		/// ��������
		/// </summary>
		struct SceneData
		{
			glm::mat4 ViewProjectionMatrix;	//vp����
		};

		static SceneData* m_SceneData;	//��������
	public:
		/// <summary>
		/// ��ʼ��Ⱦ���������ó�����������ɫ���������⣬�ƹ⣬�任�������...��
		/// </summary>
		/// <param name="camera">���</param>
		static void BeginScene(Camera& camera);

		/// <summary>
		/// ������Ⱦ����
		/// </summary>
		static void EndScene();

		/// <summary>
		/// �ύ��Ⱦ����
		/// </summary>
		/// <param name="shader">��ɫ��</param>
		/// <param name="vertexArray">����Ⱦ��������</param>
		static void Submit(const std::shared_ptr<Shader> shader, const std::shared_ptr<VertexArray>& vertexArray);
	};
}