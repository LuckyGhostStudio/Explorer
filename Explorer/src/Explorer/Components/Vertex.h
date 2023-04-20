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

		int ID;				//����ID
		int ObjectID;		//��������ID
	};
}