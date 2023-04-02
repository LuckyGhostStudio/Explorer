#pragma once

#include <glm/glm.hpp>

#include "Explorer/Renderer/Shader.h"
#include "Explorer/Renderer/Texture.h"

namespace Explorer
{
	/// <summary>
	/// �������
	/// </summary>
	class Material
	{
	public:
		/// <summary>
		/// ��ͼ����
		/// </summary>
		enum class TextureType
		{
			Albedo = 0,
			Specular = 1
		};

		glm::vec4 m_Color = { 0.8f, 0.8f, 0.8f, 1.0f };	//��ɫ
		bool m_AlbedoTextureExist = false;
		bool m_SpecularTextureExist = false;
		glm::vec2 m_Tiling = { 1.0f, 1.0f };			//����ƽ������
		glm::vec2 m_Offset = { 0.0f, 0.0f };			//����ƫ����
	private:
		std::shared_ptr<Shader> m_Shader;				//��ɫ��������ɫ������ѡ�� TODO:��ɫ�����ʼ������������ɫ��

		std::shared_ptr<Texture2D> m_AlbedoTexture;		//��������ͼ����������ͼ��������ԭ����ɫ
		std::shared_ptr<Texture2D> m_SpecularTexture;	//���淴����ͼ
		float m_Shininess = 32.0f;						//����ȣ�Ӱ�쾵��߹��ɢ��/�뾶

		//Ambient �����⣺TODO:��ӵ�Environment ����
		//Normal ������ͼ
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