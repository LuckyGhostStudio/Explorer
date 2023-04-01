#include <exppch.h>
#include "Mesh.h"

namespace Explorer
{
	Mesh::Mesh(const std::vector<SubMesh>& subMeshes, Type type) :m_SubMeshes(subMeshes), m_Type(type)
	{
		m_SubMeshCount = m_SubMeshes.size();	//����������

		//��������������
		for (SubMesh& subMesh : m_SubMeshes) {
			m_VertexCount += subMesh.GetVertexCount();				//�ۼƶ������
			m_VertexIndexCount += subMesh.GetVertexIndexCount();	//�ۼƶ�����������
		}
	}

	void Mesh::AddSubMesh(SubMesh subMesh)
	{
		m_SubMeshes.push_back(subMesh);	//���������

		m_SubMeshCount++;
		m_VertexCount += subMesh.GetVertexCount();
		m_VertexIndexCount += subMesh.GetVertexIndexCount();
	}
}