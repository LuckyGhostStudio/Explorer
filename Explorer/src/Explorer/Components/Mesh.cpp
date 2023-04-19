#include <exppch.h>
#include "Mesh.h"

#include "Explorer/Utils/ModelImporter.h"

namespace Explorer
{
	Mesh::Mesh(Type type)
	{
		SetType(type);	//设置网格类型
	}

	Mesh::Mesh(const std::vector<SubMesh>& subMeshes, Type type) :m_SubMeshes(subMeshes), m_Type(type)
	{
		m_SubMeshCount = m_SubMeshes.size();	//子网格数量

		//遍历所有子网格
		for (SubMesh& subMesh : m_SubMeshes) {
			m_VertexCount += subMesh.GetVertexCount();				//累计顶点个数
			m_VertexIndexCount += subMesh.GetVertexIndexCount();	//累计顶点索引个数
		}
	}

	void Mesh::SetType(Type type)
	{
		m_Type = type;	//设置网格类型

		if (type != Type::Other) {
			ClearSubMeshes();	//清空子网格

			//以下的.mesh文件都是.obj文件（仅仅改变后缀名，文件结构无变化，为区分内置Mesh）TODO
			switch (type)
			{
				case Type::None:
					m_Name = "[None] (Mesh)";
					break;
				case Type::Cube:
					m_Name = "Cube (Mesh)";
					ModelImporter::Load("assets/models/defaults/Cube.mesh");		//加载Cube.mesh文件
					break;
				case Type::Plane:
					m_Name = "Plane (Mesh)";
					ModelImporter::Load("assets/models/defaults/Plane.mesh");		//加载Plane.mesh文件
					break;
				case Type::Cone:
					m_Name = "Cone (Mesh)";
					ModelImporter::Load("assets/models/defaults/Cone.mesh");		//加载Cone.mesh文件
					break;
				case Type::Cylinder:
					m_Name = "Cylinder (Mesh)";
					ModelImporter::Load("assets/models/defaults/Cylinder.mesh");	//加载Cylinder.mesh文件
					break;
				case Type::Sphere:
					m_Name = "Sphere (Mesh)";
					ModelImporter::Load("assets/models/defaults/Sphere.mesh");		//加载Sphere.mesh文件
					break;
			}
			AddSubMesh(ModelImporter::GetMesh().GetSubMeshes()[0]);	//添加第0个子网格
		}
	}

	void Mesh::AddSubMesh(SubMesh subMesh)
	{
		m_SubMeshes.push_back(subMesh);	//添加子网格

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