#include "exppch.h"
#include "Texture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Explorer
{
	Texture2D::Texture2D(const std::string& path) :m_Path(path)
	{
		int width, height, channels;			//宽 高 通道数
		stbi_set_flip_vertically_on_load(1);	//垂直翻转
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);	//加载图片

		EXP_CORE_ASSERT(data, "Failed to load image!");		//加载失败

		m_Width = width;
		m_Height = height;

		GLenum internalFormat = 0, dataFormat = 0;	//内部格式（存储格式），数据格式

		if (channels == 4) {		//颜色通道4
			internalFormat = GL_RGBA8;	//每个通道8位
			dataFormat = GL_RGBA;
		}
		else if (channels == 3) {		//颜色通道3
			internalFormat = GL_RGB8;	//每个通道8位
			dataFormat = GL_RGB;
		}

		EXP_CORE_ASSERT(internalFormat & dataFormat, "Format not supported!");	//不支持此格式

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);	//创建2D纹理
		glTextureStorage2D(m_RendererID, 1, internalFormat, m_Width, m_Height);	//存储2D纹理 - - 内部格式 - -

		//设置纹理参数
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//缩小过滤器 线性插值
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	//放大过滤器 最近滤波

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, dataFormat, GL_UNSIGNED_BYTE, data);	//生成纹理到GPU

		stbi_image_free(data);	//释放data
	}
	
	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererID);	//删除纹理
	}
	
	void Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);	//绑定纹理到 slot 槽位
	}
}