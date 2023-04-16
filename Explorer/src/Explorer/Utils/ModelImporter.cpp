#include "exppch.h"
#include "ModelImporter.h"

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include "Explorer/Core/Log.h"

namespace Explorer
{
	Mesh ModelImporter::m_Mesh;
	std::string ModelImporter::m_Path;

	void ModelImporter::Load(const std::string& path)
	{
		m_Path = path;
		EXP_CORE_TRACE("Model Path:{0}", m_Path);	//模型路径

		Assimp::Importer importer;	//导入器
		//加载场景 路径 三角面化 | 翻转uv的y轴（OpenGL的uv的y轴颠倒）| 计算顶点的切线和副切线 | 如果没有法线则生成顶点法线
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate /*| aiProcess_FlipUVs*/ | aiProcess_CalcTangentSpace | aiProcess_GenNormals);

		//场景为空 | 数据不完整 | 根节点为空
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			EXP_CORE_ERROR("Assimp Load Error: {0}", importer.GetErrorString());	//加载失败
			return;
		}

		ClearData();	//清除数据
		ProcessNode(scene->mRootNode, scene);	//深度遍历处理场景结点
	}

	void ModelImporter::ProcessNode(aiNode* node, const aiScene* scene)
	{
		//处理当前结点的所有Mesh
		for (int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];	//当前结点的第i个Mesh

			m_Mesh.AddSubMesh(AiMeshToMesh(mesh, scene));	//添加子网格
		}

		//遍历当前结点的每个子节点
		for (int i = 0; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene);
		}
	}

	SubMesh ModelImporter::AiMeshToMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;	//顶点列表
		std::vector<uint32_t> indices;	//顶点索引列表

		//遍历mesh的所有顶点
		for (int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;	//顶点

			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };	//位置
			vertex.Color = { 1.0f, 0.0f, 1.0f, 1.0f };												//颜色（默认材质缺失颜色）
			vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };		//法线

			//第0个vu坐标存在
			if (mesh->mTextureCoords[0]) {
				vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };	//纹理坐标
			}
			else {
				vertex.TexCoord = { 0.0f, 0.0f };
			}
			
			vertex.ID = i;			//顶点ID
			vertex.ObjectID = 0;	//所属物体ID

			vertices.push_back(vertex);	//处理好的顶点添加到顶点列表
		}

		//遍历mesh的所有面
		for (int i = 0; i < mesh->mNumFaces; i++) {
			//遍历第i个面的索引
			for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
				indices.push_back(mesh->mFaces[i].mIndices[j]);		//添加到索引列表
			}
		}

		return SubMesh(vertices, indices);
	}

	void ModelImporter::ClearData()
	{
		m_Mesh.ClearSubMeshes();	//清空子网格
		m_Path.clear();
	}
}