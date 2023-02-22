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

		GLenum internalFormat = 0, dataFormat = 0;	//�ڲ���ʽ���洢��ʽ�������ݸ�ʽ

		if (channels == 4) {		//��ɫͨ��4
			internalFormat = GL_RGBA8;	//ÿ��ͨ��8λ
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {		//��ɫͨ��3
			internalFormat = GL_RGB8;	//ÿ��ͨ��8λ
			dataFormat = GL_RGB;
		}

		EXP_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");	//��֧�ִ˸�ʽ

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);	//����2D����
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);	//�洢2D���� - - �ڲ���ʽ - -

		//�����������
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//��С������ ���Բ�ֵ
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	//�Ŵ������ ����˲�

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);	//��������GPU

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