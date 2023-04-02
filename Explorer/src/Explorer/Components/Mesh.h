#pragma once
#include <glm/glm.hpp>

#include "Explorer/Components/SubMesh.h"

namespace Explorer
{
	/// <summary>
	/// 边
	/// </summary>
	struct Edge
	{
		int ID;	//边ID

		uint32_t VertesIndex1;	//顶点1索引
		uint32_t VertesIndex2;	//顶点2索引
	};

	/// <summary>
	/// 面
	/// </summary>
	struct Face
	{
		int ID;	//面ID

		std::vector<Edge> Edges;				//边列表

		std::vector<uint32_t> VertexIndices;	//顶点索引列表
		std::vector<uint32_t> UVIndices;		//UV索引列表
		std::vector<uint32_t> NormalIndices;	//法线索引列表
	};

	/// <summary>
	/// 网格：只保存网格数据 不做OpenGL数据处理
	/// </summary>
	class Mesh
	{
	public:
		/// <summary>
		/// 网格类型
		/// </summary>
		enum class Type
		{
			None = 0,	//无Mesh
			Other = 1,	//其他类型：不是原生Mesh

			//原生类型
			Cube = 2,		//正方体
			Sphere = 3,		//球
			Capsule = 4,	//胶囊
			Cylinder = 5,	//圆柱体
			Plane = 6		//平面
		};
	private:
		Type m_Type = Type::None;

		std::vector<SubMesh> m_SubMeshes;	//所有子网格

		uint32_t m_SubMeshCount = 0;		//子网格数

		uint32_t m_VertexCount = 0;			//顶点个数
		uint32_t m_VertexIndexCount = 0;	//顶点索引个数
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
		/// 添加子网格
		/// </summary>
		/// <param name="subMesh">子网格</param>
		void AddSubMesh(SubMesh subMesh);

		void ClearSubMeshes();

		std::vector<SubMesh>::iterator begin() { return m_SubMeshes.begin(); }
		std::vector<SubMesh>::iterator end() { return m_SubMeshes.end(); }
	};
}