#pragma once
#include <glm/glm.hpp>

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
		float TexIndex;		//纹理索引

		int ID;				//顶点ID
		int ObjectID;		//所属物体ID
	};

	/// <summary>
	/// 边
	/// </summary>
	struct Edge
	{
		int ID;	//边ID

		uint32_t VertesIndex1;	//顶点1索引
		uint32_t VertesIndex2;	//顶点2索引
	};

	/// <summary>
	/// 面
	/// </summary>
	struct Face
	{
		int ID;	//面ID

		std::vector<Edge> Edges;				//边列表

		std::vector<uint32_t> VertexIndices;	//顶点索引列表
		std::vector<uint32_t> UVIndices;		//UV索引列表
		std::vector<uint32_t> NormalIndices;	//法线索引列表
	};

	/// <summary>
	/// 网格
	/// </summary>
	class Mesh
	{
	private:
		std::vector<Vertex> m_Vertices;			//顶点列表
		std::vector<Edge> m_Edges;				//边列表
		std::vector<Face> m_Faces;				//面列表

		std::vector<uint32_t> m_VertexIndices;	//顶点索引列表
		//TODO:添加法线列表
		//TODO:添加uv列表
	public:
		Mesh() = default;
		Mesh(const std::vector<Vertex>& vertices, const std::vector<uint32_t>& indices);
	};
}