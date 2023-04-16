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
		EXP_CORE_TRACE("Model Path:{0}", m_Path);	//ģ��·��

		Assimp::Importer importer;	//������
		//���س��� ·�� �����滯 | ��תuv��y�ᣨOpenGL��uv��y��ߵ���| ���㶥������ߺ͸����� | ���û�з��������ɶ��㷨��
		const aiScene* scene = importer.ReadFile(path, aiProcess_Triangulate /*| aiProcess_FlipUVs*/ | aiProcess_CalcTangentSpace | aiProcess_GenNormals);

		//����Ϊ�� | ���ݲ����� | ���ڵ�Ϊ��
		if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode) {
			EXP_CORE_ERROR("Assimp Load Error: {0}", importer.GetErrorString());	//����ʧ��
			return;
		}

		ClearData();	//�������
		ProcessNode(scene->mRootNode, scene);	//��ȱ������������
	}

	void ModelImporter::ProcessNode(aiNode* node, const aiScene* scene)
	{
		//����ǰ��������Mesh
		for (int i = 0; i < node->mNumMeshes; i++) {
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];	//��ǰ���ĵ�i��Mesh

			m_Mesh.AddSubMesh(AiMeshToMesh(mesh, scene));	//���������
		}

		//������ǰ����ÿ���ӽڵ�
		for (int i = 0; i < node->mNumChildren; i++) {
			ProcessNode(node->mChildren[i], scene);
		}
	}

	SubMesh ModelImporter::AiMeshToMesh(aiMesh* mesh, const aiScene* scene)
	{
		std::vector<Vertex> vertices;	//�����б�
		std::vector<uint32_t> indices;	//���������б�

		//����mesh�����ж���
		for (int i = 0; i < mesh->mNumVertices; i++) {
			Vertex vertex;	//����

			vertex.Position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };	//λ��
			vertex.Color = { 1.0f, 0.0f, 1.0f, 1.0f };												//��ɫ��Ĭ�ϲ���ȱʧ��ɫ��
			vertex.Normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };		//����

			//��0��vu�������
			if (mesh->mTextureCoords[0]) {
				vertex.TexCoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };	//��������
			}
			else {
				vertex.TexCoord = { 0.0f, 0.0f };
			}
			
			vertex.ID = i;			//����ID
			vertex.ObjectID = 0;	//��������ID

			vertices.push_back(vertex);	//����õĶ�����ӵ������б�
		}

		//����mesh��������
		for (int i = 0; i < mesh->mNumFaces; i++) {
			//������i���������
			for (int j = 0; j < mesh->mFaces[i].mNumIndices; j++) {
				indices.push_back(mesh->mFaces[i].mIndices[j]);		//��ӵ������б�
			}
		}

		return SubMesh(vertices, indices);
	}

	void ModelImporter::ClearData()
	{
		m_Mesh.ClearSubMeshes();	//���������
		m_Path.clear();
	}
}