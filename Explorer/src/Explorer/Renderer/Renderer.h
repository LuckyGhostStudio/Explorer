#pragma once

#include "RenderCommand.h"
#include "Explorer/Components/Camera.h"
#include "Shader.h"

namespace Explorer
{
	/// <summary>
	/// ��Ⱦ��
	/// </summary>
	class Renderer
	{
	public:
		/// <summary>
		/// ��Ⱦ������
		/// </summary>
		enum class Type
		{
			Rasterization = 0,	//��դ��
			Raytracing = 1		//����׷��
		};

		static Type s_Type;	//��Ⱦ�����ͣ�0 ��դ����Ⱦ�� 1 ����׷����Ⱦ����
	public:
		/// <summary>
		/// ��ʼ����Ⱦ��
		/// </summary>
		static void Init();

		static void Shutdown();

		/// <summary>
		/// ��������ʱ����
		/// </summary>
		/// <param name="width">��</param>
		/// <param name="height">��</param>
		static void OnWindowResize(uint32_t width, uint32_t height);

		/// <summary>
		/// ��ʼ��Ⱦ���������ó�����������ɫ���������⣬�ƹ⣬�任�������...��
		/// </summary>
		/// <param name="camera">���</param>
		static void BeginScene(Camera& camera);

		/// <summary>
		/// ������Ⱦ����
		/// </summary>
		static void EndScene();
	};
}