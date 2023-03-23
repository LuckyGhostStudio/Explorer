#pragma once

#include "Explorer/Components/Transform.h"
#include "Explorer/Components/Camera.h"
#include "Explorer/Components/Light.h"
#include "Explorer/Components/Components.h"
#include "Explorer/Renderer/EditorCamera.h"

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
		/// <param name="camera">�������</param>
		static void BeginScene(const Camera& camera, const Transform& transform);

		/// <summary>
		/// ��ʼ��Ⱦ����
		/// </summary>
		/// <param name="camera">�༭�����</param>
		/// <param name="lightObjects">�������й�Դ����</param>
		static void BeginScene(const EditorCamera& camera, std::vector<Object>& lightObjects);

		/// <summary>
		/// ������Ⱦ����
		/// </summary>
		static void EndScene();

		/// <summary>
		/// ��ʼ����������������Ⱦ��ʼ����
		/// </summary>
		static void StartMeshProcessing();

		/// <summary>
		/// ��ʼ��һ��������������һ��������Ⱦ ��ʼ��һ��
		/// </summary>
		static void NextMeshProcessing();

		/// <summary>
		/// ������Ⱦ�������
		/// </summary>
		static void MeshProcessing();

		static void DrawMesh(const Transform& transform, int objectID = -1);

		/// <summary>
		/// ͳ������
		/// </summary>
		struct Statistics
		{
			uint32_t DrawCalls = 0;		//���Ƶ��ô���
			uint32_t TriangleCount = 0;	//�����θ���
			uint32_t VertexCount = 0;	//�������
			uint32_t IndexCount = 0;	//��������
		};

		static Statistics GetStats();

		/// <summary>
		/// ����ͳ������
		/// </summary>
		static void ResetStats();
	};
}