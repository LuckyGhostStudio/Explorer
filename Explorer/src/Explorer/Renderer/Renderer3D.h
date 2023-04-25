#pragma once

#include "Explorer/Components/Transform.h"
#include "Explorer/Components/Camera.h"
#include "Explorer/Components/Light.h"
#include "Explorer/Components/Mesh.h"
#include "Explorer/Components/Material.h"
#include "Explorer/Components/SpriteRenderer.h"
#include "Explorer/Components/CircleRenderer.h"
#include "Explorer/Components/Components.h"

#include "Explorer/Components/Line.h"
#include "Explorer/Components/Rectangle.h"

#include "Explorer/Renderer/EditorCamera.h"
#include "Explorer/Renderer/Environment.h"

#include "Explorer/Scene/Object.h"

namespace Explorer
{
	/// <summary>
	/// ��Ⱦ��������
	/// </summary>
	enum class RenderCommandType
	{
		Triangle = 0,
		Line = 1,
		Rectangle = 2
	};

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
		static void DrawCircle(const Transform& transform, CircleRenderer& circleRenderer, int objectID = -1);

		static void DrawCircle(const glm::mat4& transform, Circle& circle, int objectID = -1);

		static void DrawLine(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int objectID = -1);
		static void DrawLine(Line& line);

		static void DrawRect(const glm::vec3& position, const glm::vec2& size, const glm::vec4& color, int objectID = -1);
		static void DrawRect(const glm::mat4& transform, Rectangle& rectangle, int objectID = -1);
		
		/// <summary>
		/// ��Ⱦ����
		/// </summary>
		/// <typeparam name="T">��Ⱦ��������</typeparam>
		/// <typeparam name="V">��������</typeparam>
		/// <param name="t">��Ⱦ����</param>
		/// <param name="type">��Ⱦ��������</param>
		template<typename T, typename V>
		static void Processing(T& t, RenderCommandType type);

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
	
	template<typename T, typename V>
	inline void Renderer3D::Processing(T& t, RenderCommandType type)
	{
		uint32_t dataSize = (uint32_t)sizeof(V) * t.GetVertexBufferData().size();	//���㶥�㻺�������ݴ�С���ֽڣ�

		t.GetVertexBuffer()->SetData(t.GetVertexBufferData().data(), dataSize);	//���ö��㻺��������

		switch (type)
		{
			case RenderCommandType::Triangle:
				RenderCommand::DrawIndexed(t.GetVertexArray());		//���Ƶ��ã�����������
				break;
			case RenderCommandType::Line:
				RenderCommand::DrawLines(t.GetVertexArray(), 2);	//���Ƶ��ã�����ֱ��
				break;
			case RenderCommandType::Rectangle:
				RenderCommand::DrawLoopLine(t.GetVertexArray(), 4);	//���Ƶ��ã�����Rectangle
				break;
		}

		s_Data.Stats.DrawCalls++;	//���Ƶ��ô���++
	}
}