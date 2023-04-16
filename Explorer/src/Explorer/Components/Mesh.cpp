#include <exppch.h>
#include "Mesh.h"

namespace Explorer
{
	Mesh::Mesh(Type type)
	{
		SetType(type);	//������������
	}

	Mesh::Mesh(const std::vector<SubMesh>& subMeshes, Type type) :m_SubMeshes(subMeshes), m_Type(type)
	{
		m_SubMeshCount = m_SubMeshes.size();	//����������

		//��������������
		for (SubMesh& subMesh : m_SubMeshes) {
			m_VertexCount += subMesh.GetVertexCount();				//�ۼƶ������
			m_VertexIndexCount += subMesh.GetVertexIndexCount();	//�ۼƶ�����������
		}
	}

	void Mesh::SetType(Type type)
	{
		m_Type = type;	//������������

		if (type != Type::Other) {
			ClearSubMeshes();	//���������

			std::vector<Vertex> vertices;	//Mesh����
			std::vector<uint32_t> indices;	//Mesh��������

			switch (type)
			{
				case Type::None:
					m_Name = "[None] (Mesh)";
					break;
				case Type::Cube:
					m_Name = "Cube (Mesh)";

					//�����嶥��
					vertices =
					{
						// ---------λ��---------	----------��ɫ----------		---------����--------	---��������---  ID objID
						{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 0.0f }, 0, 0 },	// A 0 x+
						{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f }, { 0.0f, 0.0f }, 0, 0 },	// A 1 y-
						{ {  0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f }, { 0.0f, 1.0f }, 0, 0 },	// A 2 z+

						{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f },	{ 1.0f, 0.0f }, 1, 0 },	// B 3
						{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f },	{ 0.0f, 1.0f }, 1, 0 },	// B 4
						{ {  0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f },	{ 0.0f, 0.0f }, 1, 0 },	// B 5

						{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f },	{ 1.0f, 1.0f }, 2, 0 },	// C 6
						{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f },	{ 0.0f, 0.0f }, 2, 0 },	// C 7
						{ {  0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f },	{ 0.0f, 1.0f }, 2, 0 },	// C 8

						{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  1.0f,  0.0f,  0.0f }, { 0.0f, 1.0f }, 3, 0 },	// D 9
						{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f }, { 0.0f, 1.0f }, 3, 0 },	// D 10
						{ {  0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f }, { 0.0f, 0.0f }, 3, 0 },	// D 11

						{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f },	{ 1.0f, 0.0f }, 4, 0 },	// E 12
						{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f },	{ 1.0f, 0.0f }, 4, 0 },	// E 13
						{ { -0.5f, -0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f },	{ 1.0f, 1.0f }, 4, 0 },	// E 14

						{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f },	{ 0.0f, 0.0f }, 5, 0 },	// F 15
						{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f, -1.0f,  0.0f },	{ 1.0f, 1.0f }, 5, 0 },	// F 16
						{ { -0.5f, -0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f },	{ 1.0f, 0.0f }, 5, 0 },	// F 17

						{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f },	{ 0.0f, 1.0f }, 6, 0 },	// G 18
						{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f },	{ 1.0f, 0.0f }, 6, 0 },	// G 19
						{ { -0.5f,  0.5f, -0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f, -1.0f },	{ 1.0f, 1.0f }, 6, 0 },	// G 20

						{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, { -1.0f,  0.0f,  0.0f },	{ 1.0f, 1.0f }, 7, 0 },	// H 21
						{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  1.0f,  0.0f },	{ 1.0f, 1.0f }, 7, 0 },	// H 22
						{ { -0.5f,  0.5f,  0.5f }, { 1.0f, 0.0f, 1.0f, 1.0f }, {  0.0f,  0.0f,  1.0f },	{ 1.0f, 0.0f }, 7, 0 },	// H 23
					};

					//��������
					indices =
					{
						0, 3, 6,	// A B C x+
						6, 9, 0,	// C D A x+
						18, 15, 12,	// G F E x-
						18, 21, 12,	// G H E x-
						22, 7, 19,	// H C G y+
						7, 10, 22,	// C D H y+
						13, 16, 4,	// E F B y-
						4, 1, 13,	// B A E y-
						23, 14, 2,	// H E A z+
						2, 11, 23,	// A D H z+
						20, 5, 17,	// G B F z-
						5, 20, 8,	// B G C z-
					};

					AddSubMesh(SubMesh(vertices, indices));	//���������
					break;
				case Type::Plane:
					m_Name = "Plane (Mesh)";

					//TODO::���ƽ�涥������
					break;
			}
		}
	}

	void Mesh::AddSubMesh(SubMesh subMesh)
	{
		m_SubMeshes.push_back(subMesh);	//���������

		m_SubMeshCount++;
		m_VertexCount += subMesh.GetVertexCount();
		m_VertexIndexCount += subMesh.GetVertexIndexCount();
	}

	void Mesh::ClearSubMeshes()
	{
		m_SubMeshes.clear();

		m_SubMeshCount = 0;
		m_VertexCount = 0;
		m_VertexIndexCount = 0;
	}
}