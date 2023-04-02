#pragma once

#include <glm/glm.hpp>
#include "Explorer/Components/Light.h"

namespace Explorer
{
	/// <summary>
	/// ��Դ����
	/// </summary>
	struct LightData
	{
		glm::vec3 Position;
		glm::vec3 Direction;	//���շ��� z-

		Light::Type Type;		//��������    
		float Range;			//���հ뾶 Point | Spot
		float SpotOuterAngle;	//���Ž� Spot
		float SpotInnerAngle;	//���Ž� Spot

		glm::vec3 Color;		//������ɫ
		float Intensity;		//����ǿ��
		bool RenderShadow;		//�Ƿ���Ⱦ��Ӱ
	};

	/// <summary>
	/// ��������
	/// </summary>
	struct MaterialData
	{
		bool Exist = true;

		glm::vec4 Color;

		uint32_t AlbedoTextureSlot;		//��������ͼ��
		uint32_t SpecularTextureSlot;	//�߹���ͼ��

		bool AlbedoTextureExist;		//��������ͼ����
		bool SpecularTextureExist;		//�߹���ͼ����

		float Shininess;				//�����

		glm::vec2 Tiling = { 1.0f, 1.0f };	//����ƽ������
		glm::vec2 Offset = { 0.0f, 0.0f };	//����ƫ����
	};
}