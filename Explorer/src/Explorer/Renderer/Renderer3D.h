#pragma once

#include "Camera.h"
#include "Explorer/Scene/Components.h"

namespace Explorer
{
	/// <summary>
	/// 3D��Ⱦ��
	/// </summary>
	class Renderer3D
	{
	private:
		/// <summary>
		/// ��Ⱦ������
		/// </summary>
		enum class RendererType
		{
			Rasterization = 0,	//��դ��
			Raytracing = 1		//����׷��
		};

		static RendererType m_Type;	//��Ⱦ�����ͣ�0 ��դ����Ⱦ�� 1 ����׷����Ⱦ����
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
		static void BeginScene(const Camera& camera, const Transform& transform);

		/// <summary>
		/// ������Ⱦ����
		/// </summary>
		static void EndScene();

		static void DrawMesh(const Transform& transform);
	};
}