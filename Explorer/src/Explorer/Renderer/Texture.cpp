#include "exppch.h"
#include "Texture.h"

#include "stb_image.h"

#include <glad/glad.h>

namespace Explorer
{
	Texture2D::Texture2D(uint32_t width, uint32_t height) :m_Width(width), m_Height(height)
	{
		m_InternalFormat = GL_RGBA8;	//内部格式
		m_DataFormat = GL_RGBA;			//数据格式

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);		//创建2D纹理
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);	//存储2D纹理 - - 内部格式 - -

		//设置纹理参数
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//缩小过滤器 线性插值
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	//放大过滤器 最近滤波

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);		//x超过0-1 重复
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);		//y超过0-1 重复
	}

	Texture2D::Texture2D(const std::string& path) :m_Path(path)
	{
		int width, height, channels;			//宽 高 通道数
		stbi_set_flip_vertically_on_load(1);	//垂直翻转
		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);	//加载图片

		EXP_CORE_ASSERT(data, "Failed to load image!");		//加载失败

		m_Width = width;
		m_Height = height;

		if (channels == 4) {		//颜色通道4
			m_InternalFormat = GL_RGBA8;	//每个通道8位
			m_DataFormat = GL_RGBA;
		}
		else if (channels == 3) {		//颜色通道3
			m_InternalFormat = GL_RGB8;	//每个通道8位
			m_DataFormat = GL_RGB;
		}

		EXP_CORE_ASSERT(m_InternalFormat & m_DataFormat, "Format not supported!");	//不支持此格式

		glCreateTextures(GL_TEXTURE_2D, 1, &m_RendererID);	//创建2D纹理
		glTextureStorage2D(m_RendererID, 1, m_InternalFormat, m_Width, m_Height);	//存储2D纹理 - - 内部格式 - -

		//设置纹理参数
		glTextureParameteri(m_RendererID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//缩小过滤器 线性插值
		glTextureParameteri(m_RendererID, GL_TEXTURE_MAG_FILTER, GL_NEAREST);	//放大过滤器 最近滤波

		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_REPEAT);		//x超过0-1 重复
		glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_REPEAT);		//y超过0-1 重复

		//glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//x超过0-1 延伸
		//glTextureParameteri(m_RendererID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//y超过0-1 延伸

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);	//生成纹理到GPU

		stbi_image_free(data);	//释放data
	}
	
	Texture2D::~Texture2D()
	{
		glDeleteTextures(1, &m_RendererID);	//删除纹理
	}
	
	void Texture2D::SetData(void* data, uint32_t size)
	{
		uint32_t bpp = m_DataFormat == GL_RGBA ? 4 : 3;	//每个像素字节数
		EXP_CORE_ASSERT(size == m_Width * m_Height * bpp, "Data must be entire texture!");	//纹理数据不完整

		glTextureSubImage2D(m_RendererID, 0, 0, 0, m_Width, m_Height, m_DataFormat, GL_UNSIGNED_BYTE, data);	//生成纹理到GPU
	}

	void Texture2D::Bind(uint32_t slot) const
	{
		glBindTextureUnit(slot, m_RendererID);	//绑定纹理到 slot 槽位
	}
	
	Cubemap::Cubemap()
	{
		glGenTextures(1, &m_RendererID);					//创建贴图
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);	//绑定Cubemap
		
		int width, height, channels;			//宽 高 通道数

		unsigned char* data = stbi_load("assets/textures/defaults/Explorer_NoneSkybox.jpg", &width, &height, &channels, 0);	//加载默认NoneSkybox图片

		EXP_CORE_ASSERT(data, "Failed to load image!");		//加载失败
		
		m_Width = width;
		m_Height = height;
		
		for (uint32_t i = 0; i < 6; i++) {
			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);	//生成纹理到GPU
		}
		stbi_image_free(data);	//释放data
		
		//设置纹理参数
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//缩小过滤器 线性插值		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//放大过滤器 线性插值
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//x超过0-1 延伸
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//y超过0-1 延伸
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);	//z超过0-1 延伸
	}

	Cubemap::Cubemap(const std::vector<std::string>& paths) :m_Paths(paths)
	{
		glGenTextures(1, &m_RendererID);					//创建贴图
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);	//绑定Cubemap

		int width, height, channels;			//宽 高 通道数

		for (uint32_t i = 0; i < m_Paths.size(); i++) {

			unsigned char* data = stbi_load(m_Paths[i].c_str(), &width, &height, &channels, 0);	//加载图片

			EXP_CORE_ASSERT(data, "Failed to load image!");		//加载失败

			m_Width = width;
			m_Height = height;

			glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + i, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);	//生成纹理到GPU

			stbi_image_free(data);	//释放data
		}

		//设置纹理参数
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//缩小过滤器 线性插值		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//放大过滤器 线性插值
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//x超过0-1 延伸
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//y超过0-1 延伸
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);	//z超过0-1 延伸
	}
	Cubemap::~Cubemap()
	{
		glDeleteTextures(1, &m_RendererID);	//删除纹理
	}

	void Cubemap::SetSide(const std::string& path, TextureDirection directiont)
	{
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);	//绑定Cubemap
		int width, height, channels;			//宽 高 通道数

		unsigned char* data = stbi_load(path.c_str(), &width, &height, &channels, 0);	//加载图片

		m_Width = width;
		m_Height = height;

		glTexImage2D(GL_TEXTURE_CUBE_MAP_POSITIVE_X + (int)directiont, 0, GL_RGB, m_Width, m_Height, 0, GL_RGB, GL_UNSIGNED_BYTE, data);	//生成纹理到GPU

		stbi_image_free(data);	//释放data

		SetParameteri();
	}

	void Cubemap::SetParameteri()
	{
		//设置纹理参数
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MAG_FILTER, GL_LINEAR);		//缩小过滤器 线性插值		
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_MIN_FILTER, GL_LINEAR);		//放大过滤器 线性插值
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);	//x超过0-1 延伸
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);	//y超过0-1 延伸
		glTexParameteri(GL_TEXTURE_CUBE_MAP, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);	//z超过0-1 延伸
	}

	void Cubemap::SetData(void* data, uint32_t size)
	{
		
	}

	void Cubemap::Bind(uint32_t slot) const
	{
		glActiveTexture(GL_TEXTURE0 + slot);
		glBindTexture(GL_TEXTURE_CUBE_MAP, m_RendererID);	//绑定Cubemap
	}
}