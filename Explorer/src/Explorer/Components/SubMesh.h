#pragma once

#include <glm/glm.hpp>

#include "Explorer/Renderer/VertexArray.h"
#include "Explorer/Renderer/Shader.h"

namespace Explorer
{
	/// <summary>
	/// 顶点
	/// </summary>
	struct Vertex
	{
		glm::vec3 Position;	//位置
		glm::vec4 Color;	//颜色
		glm::vec3 Normal;	//法线
		glm::vec2 TexCoord;	//纹理坐标
		//float TexIndex;		//纹理索引

		int ID;				//顶点ID
		int ObjectID;		//所属物体ID
	};

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
	public:
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

		/// <summary>
		/// 返回VAO
		/// </summary>
		/// <returns>顶点数组</returns>
		const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }

		std::vector<Vertex>::iterator begin() { return m_Vertices.begin(); }
		std::vector<Vertex>::iterator end() { return m_Vertices.end(); }
	};
}