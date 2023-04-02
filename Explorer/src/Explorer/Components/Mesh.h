#pragma once
#include <glm/glm.hpp>

#include "Explorer/Components/SubMesh.h"

namespace Explorer
{
	/// <summary>
	/// ��
	/// </summary>
	struct Edge
	{
		int ID;	//��ID

		uint32_t VertesIndex1;	//����1����
		uint32_t VertesIndex2;	//����2����
	};

	/// <summary>
	/// ��
	/// </summary>
	struct Face
	{
		int ID;	//��ID

		std::vector<Edge> Edges;				//���б�

		std::vector<uint32_t> VertexIndices;	//���������б�
		std::vector<uint32_t> UVIndices;		//UV�����б�
		std::vector<uint32_t> NormalIndices;	//���������б�
	};

	/// <summary>
	/// ����ֻ������������ ����OpenGL���ݴ���
	/// </summary>
	class Mesh
	{
	public:
		/// <summary>
		/// ��������
		/// </summary>
		enum class Type
		{
			None = 0,	//��Mesh
			Other = 1,	//�������ͣ�����ԭ��Mesh

			//ԭ������
			Cube = 2,		//������
			Sphere = 3,		//��
			Capsule = 4,	//����
			Cylinder = 5,	//Բ����
			Plane = 6		//ƽ��
		};
	private:
		Type m_Type = Type::None;

		std::vector<SubMesh> m_SubMeshes;	//����������

		uint32_t m_SubMeshCount = 0;		//��������

		uint32_t m_VertexCount = 0;			//�������
		uint32_t m_VertexIndexCount = 0;	//������������
	public:
		Mesh() = default;
		Mesh(Type type);
		Mesh(const Mesh&) = default;
		Mesh(const std::vector<SubMesh>& subMeshes, Type type = Type::None);

		Type GetType() const { return m_Type; }
		void SetType(Type type);

		std::vector<SubMesh>& GetSubMeshes() { return m_SubMeshes; }

		uint32_t GetSubMeshCount() const { return m_SubMeshCount; }
		uint32_t GetVertexCount() const { return m_VertexCount; }
		uint32_t GetVertexIndexCount() const { return m_VertexIndexCount; }

		/// <summary>
		/// ���������
		/// </summary>
		/// <param name="subMesh">������</param>
		void AddSubMesh(SubMesh subMesh);

		void ClearSubMeshes();

		std::vector<SubMesh>::iterator begin() { return m_SubMeshes.begin(); }
		std::vector<SubMesh>::iterator end() { return m_SubMeshes.end(); }
	};
}