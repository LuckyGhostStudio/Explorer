#pragma once
#include <glm/glm.hpp>

#include "Explorer/Components/SubMesh.h"

#include "Explorer/Components/Component.h"

namespace Explorer
{
	/// <summary>
	/// 网格：只保存网格数据 不做OpenGL数据处理
	/// </summary>
	class Mesh :public Component
	{
	public:
		/// <summary>
		/// 网格类型
		/// </summary>
		enum class Type
		{
			None = 0,		//无Mesh
			Other = 1,		//其他类型：不是原生Mesh

			//内置类型
			Cube = 2,		//正方体
			Plane = 3,		//平面
			Cone = 4,		//圆锥
			Cylinder = 5,	//圆柱
			Sphere = 6		//经纬球
		};
	private:
		Type m_Type = Type::None;

		std::vector<SubMesh> m_SubMeshes;	//所有子网格

		uint32_t m_SubMeshCount = 0;		//子网格数

		uint32_t m_VertexCount = 0;			//顶点个数
		uint32_t m_VertexIndexCount = 0;	//顶点索引个数
	private:
		/// <summary>
		/// 设置Mesh组件图标
		/// </summary>
		virtual void SetIcon() override { m_Icon = std::make_shared<Texture2D>("Resources/Icons/Components/Mesh_Icon.png"); }
	public:
		Mesh() = default;
		Mesh(Type type);
		Mesh(const Mesh&) = default;
		Mesh(const std::vector<SubMesh>& subMeshes, Type type = Type::None);

		void SetName(const std::string& name) { m_Name = name + " (Mesh)"; }
		virtual const std::string& GetName() override { return m_Name; }
		virtual std::shared_ptr<Texture2D>& GetIcon() override { SetIcon(); return m_Icon; }

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