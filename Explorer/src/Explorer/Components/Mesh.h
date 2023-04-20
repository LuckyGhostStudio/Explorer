#pragma once
#include <glm/glm.hpp>

#include "Explorer/Components/SubMesh.h"

#include "Explorer/Components/Component.h"

namespace Explorer
{
	/// <summary>
	/// ����ֻ������������ ����OpenGL���ݴ���
	/// </summary>
	class Mesh :public Component
	{
	public:
		/// <summary>
		/// ��������
		/// </summary>
		enum class Type
		{
			None = 0,		//��Mesh
			Other = 1,		//�������ͣ�����ԭ��Mesh

			//��������
			Cube = 2,		//������
			Plane = 3,		//ƽ��
			Cone = 4,		//Բ׶
			Cylinder = 5,	//Բ��
			Sphere = 6		//��γ��
		};
	private:
		Type m_Type = Type::None;

		std::vector<SubMesh> m_SubMeshes;	//����������

		uint32_t m_SubMeshCount = 0;		//��������

		uint32_t m_VertexCount = 0;			//�������
		uint32_t m_VertexIndexCount = 0;	//������������
	private:
		/// <summary>
		/// ����Mesh���ͼ��
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
		/// ���������
		/// </summary>
		/// <param name="subMesh">������</param>
		void AddSubMesh(SubMesh subMesh);

		void ClearSubMeshes();

		std::vector<SubMesh>::iterator begin() { return m_SubMeshes.begin(); }
		std::vector<SubMesh>::iterator end() { return m_SubMeshes.end(); }
	};
}