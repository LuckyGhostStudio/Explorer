#pragma once

#include "Explorer/Renderer/Shader.h"
#include "Explorer/Renderer/Texture.h"

namespace Explorer
{
	/// <summary>
	/// 材质组件
	/// </summary>
	class Material
	{
	private:
		std::shared_ptr<Shader> m_Shader;				//着色器

		std::shared_ptr<Texture2D> m_AlbedoTexture;		//反照率贴图（漫反射贴图）：物体原生颜色
		std::shared_ptr<Texture2D> m_SpecularTexture;	//镜面反射贴图
		float m_Shininess = 32.0f;						//反光度：影响镜面高光的散射/半径

		//Ambient 环境光：TODO:添加到Environment 设置
		//Normal 法线贴图
	public:

	};
}