#include "exppch.h"
#include "Texture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Explorer
{
	Texture2D::Texture2D(uint32_t width, uint32_t height) :m_Width(width), m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;	//�ڲ���ʽ
		m_DataFormat = GL_RGBA;			//���ݸ�ʽ

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);		//����2D����
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);	//�洢2D���� - - �ڲ���ʽ - -

		//�����������
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//��С������ ���Բ�ֵ
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	//�Ŵ������ ����˲�

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);		//x����0-1 �ظ�
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);		//y����0-1 �ظ�
	}

	Texture2D::Texture2D(const std::string& path) :m_Path(path)
	{
		int width, height, channels;			//�� �� ͨ����
		stbi_set_flip_vertically_on_load(1);	//��ֱ��ת
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);	//����ͼƬ

		EXP_CORE_ASSERT(data, "Failed to load image!");		//����ʧ��

		m_Width = width;
		m_Height = height;

		if (channels == 4) {		//��ɫͨ��4
			m_InternalFormat = GL_RGBA8;	//ÿ��ͨ��8λ
			m_DataFormat = GL_RGBA;
		}
		else if (channels == 3) {		//��ɫͨ��3
			m_InternalFormat = GL_RGB8;	//ÿ��ͨ��8λ
			m_DataFormat = GL_RGB;
		}

		EXP_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!");	//��֧�ִ˸�ʽ

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);	//����2D����
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);	//�洢2D���� - - �ڲ���ʽ - -

		//�����������
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//��С������ ���Բ�ֵ
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	//�Ŵ������ ����˲�

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);		//x����0-1 �ظ�
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);		//y����0-1 �ظ�

		//glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//x����0-1 ����
		//glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//y����0-1 ����

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);	//��������GPU

		stbi_image_free(data);	//�ͷ�data
	}
	
	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererID);	//ɾ������
	}
	
	void Texture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;	//ÿ�������ֽ���
		EXP_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");	//�������ݲ�����

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);	//��������GPU
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);	//������ slot ��λ
	}
	
	Cubemap::Cubemap()
	{
		glGenTextures(1, &m_RendererID);					//������ͼ
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);	//��Cubemap
		
		int width, height, channels;			//�� �� ͨ����

		unsigned char* data = stbi_load("assets/textures/defaults/Explorer_NoneSkybox.jpg", &width, &height, &channels, 0);	//����Ĭ��NoneSkyboxͼƬ

		EXP_CORE_ASSERT(data, "Failed to load image!");		//����ʧ��
		
		m_Width = width;
		m_Height = height;
		
		for (uint32_t i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);	//��������GPU
		}
		stbi_image_free(data);	//�ͷ�data
		
		//�����������
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//��С������ ���Բ�ֵ		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//�Ŵ������ ���Բ�ֵ
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//x����0-1 ����
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//y����0-1 ����
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);	//z����0-1 ����
	}

	Cubemap::Cubemap(const std::vector<std::string>& paths) :m_Paths(paths)
	{
		glGenTextures(1, &m_RendererID);					//������ͼ
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);	//��Cubemap

		int width, height, channels;			//�� �� ͨ����

		for (uint32_t i = 0; i < m_Paths.size(); i++) {

			unsigned char* data = stbi_load(m_Paths[i].c_str(), &width, &height, &channels, 0);	//����ͼƬ

			EXP_CORE_ASSERT(data, "Failed to load image!");		//����ʧ��

			m_Width = width;
			m_Height = height;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);	//��������GPU

			stbi_image_free(data);	//�ͷ�data
		}

		//�����������
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//��С������ ���Բ�ֵ		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//�Ŵ������ ���Բ�ֵ
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//x����0-1 ����
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//y����0-1 ����
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);	//z����0-1 ����
	}
	Cubemap::~Cubemap()
	{
		glDeleteTextures(1, &m_RendererID);	//ɾ������
	}

	void Cubemap::SetSide(const std::string& path, TextureDirection directiont)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);	//��Cubemap
		int width, height, channels;			//�� �� ͨ����

		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);	//����ͼƬ

		m_Width = width;
		m_Height = height;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (int)directiont, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);	//��������GPU

		stbi_image_free(data);	//�ͷ�data

		SetParameteri();
	}

	void Cubemap::SetParameteri()
	{
		//�����������
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//��С������ ���Բ�ֵ		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//�Ŵ������ ���Բ�ֵ
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//x����0-1 ����
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//y����0-1 ����
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);	//z����0-1 ����
	}

	void Cubemap::SetData(void* data, uint32_t size)
	{
		
	}

	void Cubemap::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);	//��Cubemap
	}
}