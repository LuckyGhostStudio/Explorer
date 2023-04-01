#pragma once

#include <glm/glm.hpp>

#include "Explorer/Renderer/VertexArray.h"
#include "Explorer/Renderer/Shader.h"

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
		//float TexIndex;		//��������

		int ID;				//����ID
		int ObjectID;		//��������ID
	};

	/// <summary>
	/// ������
	/// </summary>
	class SubMesh
	{
	private:
		uint32_t m_VertexCount = 0;			//�������
		uint32_t m_VertexIndexCount = 0;	//������������

		std::vector<Vertex> m_Vertices;			//�����б�
		std::vector<uint32_t> m_VertexIndices;	//���������б� 
	public:
		std::vector<Vertex> m_VertexBufferData;	//���㻺�������ݣ�����Ҫ��Ⱦ�Ķ�������

		std::shared_ptr<VertexArray> m_VertexArray;		//�������� VAO
		std::shared_ptr<VertexBuffer> m_VertexBuffer;	//���㻺���� VBO
	public:
		SubMesh() = default;
		SubMesh(const SubMesh&) = default;
		SubMesh(const std::vector<Vertex>&vertices, const std::vector<uint32_t>&indices);
		~SubMesh();

		uint32_t GetVertexCount() const { return m_VertexCount; }
		uint32_t GetVertexIndexCount() const { return m_VertexIndexCount; }

		std::vector<Vertex>& GetVertices() { return m_Vertices; }
		std::vector<uint32_t>& GetVertexIndices() { return m_VertexIndices; }

		/// <summary>
		/// ����VAO
		/// </summary>
		/// <returns>��������</returns>
		const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }

		std::vector<Vertex>::iterator begin() { return m_Vertices.begin(); }
		std::vector<Vertex>::iterator end() { return m_Vertices.end(); }
	};
}