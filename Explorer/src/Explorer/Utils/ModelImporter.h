#pragma once

#include "Explorer/Components/Mesh.h"
#include "Explorer/Components/Material.h"

struct aiNode;
struct aiScene;
struct aiMesh;

namespace Explorer
{
	/// <summary>
	/// 模型导入器
	/// </summary>
	class ModelImporter
	{
	private:
		static Mesh m_Mesh;			//网格

		static std::string m_Path;	//模型路径
	private:
		/// <summary>
		/// 处理结点（深度优先遍历）：将结点网格加载到Mesh
		/// </summary>
		/// <param name="node">当前结点</param>
		/// <param name="scene">场景</param>
		static void ProcessNode(aiNode* node, const aiScene* scene);

		/// <summary>
		/// aiMesh转换为SubMesh
		/// </summary>
		/// <param name="mesh">读取到的aiMesh</param>
		/// <param name="scene">场景</param>
		/// <returns>处理后的子网格</returns>
		static SubMesh AiMeshToMesh(aiMesh* mesh, const aiScene* scene);

		/// <summary>
		/// 清除数据
		/// </summary>
		static void ClearData();
	public:
		/// <summary>
		/// 加载模型
		/// </summary>
		/// <param name="path">路径</param>
		static void Load(const std::string& path);

		static Mesh GetMesh() { return m_Mesh; }
	};
}