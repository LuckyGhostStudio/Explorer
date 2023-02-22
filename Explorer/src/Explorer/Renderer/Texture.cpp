#include "exppch.h"
#include "Texture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Explorer
{
	Texture2D::Texture2D(const std::string& path) :m_Path(path)
	{
		int width, height, channels;			//�� �� ͨ����
		stbi_set_flip_vertically_on_load(1);	//��ֱ��ת
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);	//����ͼƬ

		EXP_CORE_ASSERT(data, "Failed to load image!");		//����ʧ��

		m_Width = width;
		m_Height = height;

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);	//����2D����
		glTextureStorage2D(m_RendererID, 1, GL_RGB8, m_Width, m_Height);	//�洢2D���� - - ÿ��ͨ��8λ - -

		//�����������
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//��С������ ���Բ�ֵ
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	//�Ŵ������ ����˲�

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, GL_RGB, GL_UNSIGNED_BYTE, data);	//��������GPU

		stbi_image_free(data);	//�ͷ�data
	}
	
	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererID);	//ɾ������
	}
	
	void Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);	//������ slot ��λ
	}
}