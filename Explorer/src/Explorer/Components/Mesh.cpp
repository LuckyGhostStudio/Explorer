#include <exppch.h>
#include "Mesh.h"

#include "Explorer/Utils/ModelImporter.h"

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

			//���µ�.mesh�ļ�����.obj�ļ��������ı��׺�����ļ��ṹ�ޱ仯��Ϊ��������Mesh��TODO
			switch (type)
			{
				case Type::None:
					m_Name = "[None] (Mesh)";
					break;
				case Type::Cube:
					m_Name = "Cube (Mesh)";
					ModelImporter::Load("assets/models/defaults/Cube.mesh");		//����Cube.mesh�ļ�
					break;
				case Type::Plane:
					m_Name = "Plane (Mesh)";
					ModelImporter::Load("assets/models/defaults/Plane.mesh");		//����Plane.mesh�ļ�
					break;
				case Type::Cone:
					m_Name = "Cone (Mesh)";
					ModelImporter::Load("assets/models/defaults/Cone.mesh");		//����Cone.mesh�ļ�
					break;
				case Type::Cylinder:
					m_Name = "Cylinder (Mesh)";
					ModelImporter::Load("assets/models/defaults/Cylinder.mesh");	//����Cylinder.mesh�ļ�
					break;
				case Type::Sphere:
					m_Name = "Sphere (Mesh)";
					ModelImporter::Load("assets/models/defaults/Sphere.mesh");		//����Sphere.mesh�ļ�
					break;
			}
			AddSubMesh(ModelImporter::GetMesh().GetSubMeshes()[0]);	//��ӵ�0��������
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