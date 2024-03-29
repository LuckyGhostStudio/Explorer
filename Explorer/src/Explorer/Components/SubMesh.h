#pragma once

#include <glm/glm.hpp>

#include "Explorer/Renderer/VertexArray.h"
#include "Explorer/Renderer/Shader.h"

#include "Vertex.h"

namespace Explorer
{
	/// <summary>
	/// 子网格
	/// </summary>
	class SubMesh
	{
	private:
		uint32_t m_VertexCount = 0;			//顶点个数
		uint32_t m_VertexIndexCount = 0;	//顶点索引个数

		std::vector<Vertex> m_Vertices;			//顶点列表
		std::vector<uint32_t> m_VertexIndices;	//顶点索引列表 

		std::vector<Vertex> m_VertexBufferData;	//顶点缓冲区数据：最终要渲染的顶点数据

		std::shared_ptr<VertexArray> m_VertexArray;		//顶点数组 VAO
		std::shared_ptr<VertexBuffer> m_VertexBuffer;	//顶点缓冲区 VBO
	public:
		SubMesh() = default;
		SubMesh(const SubMesh&) = default;
		SubMesh(const std::vector<Vertex>&vertices, const std::vector<uint32_t>&indices);
		~SubMesh();

		uint32_t GetVertexCount() const { return m_VertexCount; }
		uint32_t GetVertexIndexCount() const { return m_VertexIndexCount; }

		std::vector<Vertex>& GetVertices() { return m_Vertices; }
		std::vector<uint32_t>& GetVertexIndices() { return m_VertexIndices; }

		std::vector<Vertex>& GetVertexBufferData() { return m_VertexBufferData; }

		const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }

		std::vector<Vertex>::iterator begin() { return m_Vertices.begin(); }
		std::vector<Vertex>::iterator end() { return m_Vertices.end(); }
	};
}