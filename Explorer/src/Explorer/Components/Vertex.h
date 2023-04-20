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

		int ID;				//顶点ID
		int ObjectID;		//所属物体ID
	};
}