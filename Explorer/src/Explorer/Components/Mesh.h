#pragma once
#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// ����
	/// </summary>
	struct Vertex
	{
		glm::vec3 Position;	//λ��
		glm::vec4 Color;	//��ɫ
		glm::vec3 Normal;	//����
		glm::vec2 TexCoord;	//��������
		float TexIndex;		//��������

		int ID;				//����ID
		int ObjectID;		//��������ID
	};

	/// <summary>
	/// ��
	/// </summary>
	struct Edge
	{
		int ID;	//��ID

		uint32_t VertesIndex1;	//����1����
		uint32_t VertesIndex2;	//����2����
	};

	/// <summary>
	/// ��
	/// </summary>
	struct Face
	{
		int ID;	//��ID

		std::vector<Edge> Edges;				//���б�

		std::vector<uint32_t> VertexIndices;	//���������б�
		std::vector<uint32_t> UVIndices;		//UV�����б�
		std::vector<uint32_t> NormalIndices;	//���������б�
	};

	/// <summary>
	/// ����
	/// </summary>
	class Mesh
	{
	private:
		std::vector<Vertex> m_Vertices;			//�����б�
		std::vector<Edge> m_Edges;				//���б�
		std::vector<Face> m_Faces;				//���б�

		std::vector<uint32_t> m_VertexIndices;	//���������б�
		//TODO:��ӷ����б�
		//TODO:���uv�б�
	public:
		Mesh() = default;
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
	};
}