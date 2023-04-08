#pragma once

#include <glm/glm.hpp>

#include "Explorer/Renderer/Shader.h"
#include "Explorer/Renderer/Texture.h"
#include "Skybox.h"

namespace Explorer
{
	/// <summary>
	/// �������
	/// </summary>
	class Material
	{
	private:
		std::shared_ptr<Shader> m_Shader;				//��ɫ��
	public:
		/// <summary>
		/// ��ͼ����
		/// </summary>
		enum class TextureType
		{
			Albedo = 0,
			Specular = 1
		};
	private:
		glm::vec4 m_Color = { 0.8f, 0.8f, 0.8f, 1.0f };	//��ɫ
		std::shared_ptr<Texture2D> m_AlbedoTexture;		//��������ͼ����������ͼ��������ԭ����ɫ
		std::shared_ptr<Texture2D> m_SpecularTexture;	//���淴����ͼ
		float m_Shininess = 32.0f;						//����ȣ�Ӱ�쾵��߹��ɢ��/�뾶
		//Normal ������ͼ
		
		glm::vec2 m_Tiling = { 1.0f, 1.0f };			//����ƽ������
		glm::vec2 m_Offset = { 0.0f, 0.0f };			//����ƫ����
		
		bool m_AlbedoTextureExist = false;
		bool m_SpecularTextureExist = false;
	public:
		Material();

		void BindTexture();
		
		glm::vec4& GetColor() { return m_Color; }
		void SetColor(const glm::vec4& color) { m_Color = color; }

		void SetAlbedoTexture(const std::string& filepath);
		void SetSpecularTexture(const std::string& filepath);

		std::shared_ptr<Texture2D>& GetAlbedoTexture() { return m_AlbedoTexture; }
		std::shared_ptr<Texture2D>& GetSpecularTexture() { return m_SpecularTexture; }

		std::shared_ptr<Shader>& GetShader() { return m_Shader; }
		void SetShader(std::shared_ptr<Shader>& shader) { m_Shader = shader; m_Shader->Bind(); }

		uint32_t GetAlbedoTextureID() { if (m_AlbedoTexture != nullptr) return m_AlbedoTexture->GetRendererID(); }
		uint32_t GetSpecularTextureID() { if (m_SpecularTexture != nullptr) return m_SpecularTexture->GetRendererID(); }

		float GetShininess() const { return m_Shininess; }
		float& GetShininess_Ref() { return m_Shininess; }
		void SetShininess(float shininess) { m_Shininess = shininess; }

		glm::vec2& GetTiling() { return m_Tiling; }
		void SetTiling(const glm::vec2& tiling) { m_Tiling = tiling; }

		glm::vec2& GetOffset() { return m_Offset; }
		void SetOffset(const glm::vec2& offset) { m_Offset = offset; }

		bool GetAlbedoTextureExist() const { return m_AlbedoTextureExist; }
		void SetAlbedoTextureExist(bool exist) { m_AlbedoTextureExist = exist; }

		bool GetSpecularTextureExist() const { return m_SpecularTextureExist; }
		void SetSpecularTextureExist(bool exist) { m_SpecularTextureExist = exist; }
	};
}