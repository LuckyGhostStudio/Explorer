#pragma once

#include "Explorer/Components/Transform.h"
#include "Explorer/Components/Camera.h"
#include "Explorer/Components/Light.h"
#include "Explorer/Components/Mesh.h"
#include "Explorer/Components/Material.h"
#include "Explorer/Components/SpriteRenderer.h"
#include "Explorer/Components/Components.h"

#include "Explorer/Renderer/EditorCamera.h"
#include "Explorer/Renderer/Environment.h"

#include "Explorer/Scene/Object.h"

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

		static RendererType s_Type;	//��Ⱦ�����ͣ�0 ��դ����Ⱦ�� 1 ����׷����Ⱦ����
	public:
		/// <summary>
		/// ��ʼ����Ⱦ��
		/// </summary>
		static void Init();

		static void Shutdown();

		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const Camera& camera, Transform& transform);
		static void DrawSprite(const Transform& transform, SpriteRenderer& spriteRenderer, /*Material& material,*/ int objectID = -1);
		
		template<typename T>
		static void Processing(T& t);

		/// <summary>
		/// ��ʼ��Ⱦ���������ó�������
		/// </summary>
		/// <param name="camera">�������</param>
		static void BeginScene(Environment& environment, const Camera& camera, Transform& transform, std::vector<Object>& lightObjects);

		/// <summary>
		/// ��ʼ��Ⱦ����
		/// </summary>
		/// <param name="environment">��������</param>
		/// <param name="camera">�༭�����</param>
		/// <param name="lightObjects">�������й�Դ����</param>
		static void BeginScene(Environment& environment, const EditorCamera& camera, std::vector<Object>& lightObjects);

		/// <summary>
		/// ������Ⱦ����
		/// </summary>
		static void EndScene(Environment& environment, const EditorCamera& camera);
		static void EndScene(Environment& environment, const Camera& camera, Transform& transform);

		static void EndScene();

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="subMesh">������</param>
		static void SubMeshProcessing(SubMesh& subMesh);

		static void DrawMesh(const Transform& transform, Mesh& mesh, Material& material, int objectID = -1);

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
	
	template<typename T>
	inline void Renderer3D::Processing(T& t)
	{
		uint32_t dataSize = (uint32_t)sizeof(Vertex) * t.GetVertexBufferData().size();	//���㶥�㻺�������ݴ�С���ֽڣ�

		t.GetVertexBuffer()->SetData(t.GetVertexBufferData().data(), dataSize);	//���ö��㻺��������

		RenderCommand::DrawIndexed(t.GetVertexArray());	//���Ƶ���

		s_Data.Stats.DrawCalls++;	//���Ƶ��ô���++
	}
}