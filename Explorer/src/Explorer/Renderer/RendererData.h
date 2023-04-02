#pragma once

#include <glm/glm.hpp>
#include "Explorer/Components/Light.h"

namespace Explorer
{
	/// <summary>
	/// 光源数据
	/// </summary>
	struct LightData
	{
		glm::vec3 Position;
		glm::vec3 Direction;	//光照方向 z-

		Light::Type Type;		//光照类型    
		float Range;			//光照半径 Point | Spot
		float SpotOuterAngle;	//外张角 Spot
		float SpotInnerAngle;	//内张角 Spot

		glm::vec3 Color;		//光照颜色
		float Intensity;		//光照强度
		bool RenderShadow;		//是否渲染阴影
	};

	/// <summary>
	/// 材质数据
	/// </summary>
	struct MaterialData
	{
		bool Exist = true;

		glm::vec4 Color;

		uint32_t AlbedoTextureSlot;		//反照率贴图槽
		uint32_t SpecularTextureSlot;	//高光贴图槽

		bool AlbedoTextureExist;		//反照率贴图存在
		bool SpecularTextureExist;		//高光贴图存在

		float Shininess;				//反光度

		glm::vec2 Tiling = { 1.0f, 1.0f };	//纹理平铺因子
		glm::vec2 Offset = { 0.0f, 0.0f };	//纹理偏移量
	};
}