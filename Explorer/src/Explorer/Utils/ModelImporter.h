#pragma once

#include "Explorer/Components/Mesh.h"
#include "Explorer/Components/Material.h"

struct aiNode;
struct aiScene;
struct aiMesh;

namespace Explorer
{
	/// <summary>
	/// ģ�͵�����
	/// </summary>
	class ModelImporter
	{
	private:
		static Mesh m_Mesh;			//����

		static std::string m_Path;	//ģ��·��
	private:
		/// <summary>
		/// �����㣨������ȱ������������������ص�Mesh
		/// </summary>
		/// <param name="node">��ǰ���</param>
		/// <param name="scene">����</param>
		static void ProcessNode(aiNode* node, const aiScene* scene);

		/// <summary>
		/// aiMeshת��ΪSubMesh
		/// </summary>
		/// <param name="mesh">��ȡ����aiMesh</param>
		/// <param name="scene">����</param>
		/// <returns>������������</returns>
		static SubMesh AiMeshToMesh(aiMesh* mesh, const aiScene* scene);

		/// <summary>
		/// �������
		/// </summary>
		static void ClearData();
	public:
		/// <summary>
		/// ����ģ��
		/// </summary>
		/// <param name="path">·��</param>
		static void Load(const std::string& path);

		static Mesh GetMesh() { return m_Mesh; }
	};
}