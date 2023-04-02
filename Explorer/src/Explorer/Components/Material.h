#pragma once

#include <glm/glm.hpp>

#include "Explorer/Renderer/Shader.h"
#include "Explorer/Renderer/Texture.h"

namespace Explorer
{
	/// <summary>
	/// 材质组件
	/// </summary>
	class Material
	{
	public:
		/// <summary>
		/// 贴图类型
		/// </summary>
		enum class TextureType
		{
			Albedo = 0,
			Specular = 1
		};

		glm::vec4 m_Color = { 0.8f, 0.8f, 0.8f, 1.0f };	//颜色
		bool m_AlbedoTextureExist = false;
		bool m_SpecularTextureExist = false;
		glm::vec2 m_Tiling = { 1.0f, 1.0f };			//纹理平铺因子
		glm::vec2 m_Offset = { 0.0f, 0.0f };			//纹理偏移量
	private:
		std::shared_ptr<Shader> m_Shader;				//着色器：从着色器库中选择 TODO:着色器库初始化加载所有着色器

		std::shared_ptr<Texture2D> m_AlbedoTexture;		//反照率贴图（漫反射贴图）：物体原生颜色
		std::shared_ptr<Texture2D> m_SpecularTexture;	//镜面反射贴图
		float m_Shininess = 32.0f;						//反光度：影响镜面高光的散射/半径

		//Ambient 环境光：TODO:添加到Environment 设置
		//Normal 法线贴图
	public:
		Material();

		void SetAlbedoTexture(const std::string& filepath);
		void SetSpecularTexture(const std::string& filepath);

		std::shared_ptr<Texture2D>& GetAlbedoTexture() { return m_AlbedoTexture; }
		std::shared_ptr<Texture2D>& GetSpecularTexture() { return m_SpecularTexture; }

		void BindTexture();

		std::shared_ptr<Shader>& GetShader() { return m_Shader; }
		void SetShader(std::shared_ptr<Shader>& shader) { m_Shader = shader; }

		uint32_t GetAlbedoTextureID() { if (m_AlbedoTexture != nullptr) return m_AlbedoTexture->GetRendererID(); }
		uint32_t GetSpecularTextureID() { if (m_SpecularTexture != nullptr) return m_SpecularTexture->GetRendererID(); }

		float GetShininess() const { return m_Shininess; }
		void SetShininess(float shininess) { m_Shininess = shininess; }
	};
}