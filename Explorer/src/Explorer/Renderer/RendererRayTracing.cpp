#include "exppch.h"
#include "RendererRayTracing.h"

#include "Explorer/Utils/Random.h"

namespace Explorer
{
	namespace Utils
	{
		/// <summary>
		/// vec4转换为32位RBGA（16进制）
		/// </summary>
		/// <param name="color">颜色</param>
		/// <returns>32位16进制颜色</returns>
		static uint32_t ConvertToRGBA(const glm::vec4& color)
		{
			uint8_t r = (uint8_t)(color.r * 255.0f);	//红色通道
			uint8_t g = (uint8_t)(color.g * 255.0f);	//绿色通道
			uint8_t b = (uint8_t)(color.b * 255.0f);	//蓝色通道
			uint8_t a = (uint8_t)(color.a * 255.0f);	//透明通道

			uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;	//ABGR

			return result;
		}
	}

	void RendererRayTracing::OnResize(uint32_t width, uint32_t height)
	{
		//图像存在
		if (m_FinalImage){
			if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height) return;

			//m_FinalImage->Resize(width, height, m_ImageData);	//重置大小
			m_FinalImage = std::make_shared<Texture2D>(width, height);	//创建图像
		}
		else{	//不存在
			m_FinalImage = std::make_shared<Texture2D>(width, height);	//创建图像
		}

		delete[] m_ImageData;
		m_ImageData = new uint32_t[width * height];
	}

	void RendererRayTracing::Render()
	{
		float aspectRatio = m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight();	//宽高比

		//遍历每个像素
		for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++){
			for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++){
				glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };	//坐标[0, 1]
				coord = coord * 2.0f - 1.0f;	//coord = [-1, 1] 相机在屏幕中心(0, 0)位置
				coord.x *= aspectRatio;

				glm::vec4 color = PerPixel(coord);	//coord坐标的像素颜色
				color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));	//限制color = [0, 1]
				m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);	//图像数据
			}
		}
		
		m_FinalImage->SetData(m_ImageData, m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(uint32_t));
	}

	glm::vec4 RendererRayTracing::PerPixel(glm::vec2 coord)
	{
		glm::vec3 rayOrigin(0.0f, 0.0f, 1.0f);				//射线源 相机位置
		glm::vec3 rayDirection(coord.x, coord.y, -1.0f);	//射线方向：从相机（0, 0）指向每个像素坐标
		float radius = 0.5f;								//球半径

		float a = glm::dot(rayDirection, rayDirection);				//二次项系数：direction.x ^ 2 + direction.y ^ 2 + direction.z ^ 2
		float b = 2.0f * glm::dot(rayOrigin, rayDirection);			//一次项系数：2 * (origin.x * direction.x + origin.y * direction.y + origin.z * direction.z)
		float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;	//常数项：origin.x ^ 2 + origin.y ^ 2 + origin.z ^ 2 - radius ^ 2

		float discriminant = b * b - 4.0f * a * c;		//射线与球交点判别式

		//无交点
		if (discriminant < 0.0f) {
			return glm::vec4(0.1, 0.1, 0.1, 1);
		}

		float t0 = (-b + glm::sqrt(discriminant)) / 2.0f * a;		//源点与交点0距离
		float closetT = (-b - glm::sqrt(discriminant)) / 2.0f * a;	//源点与最近交点的距离

		glm::vec3 hitPoint = rayOrigin + rayDirection * closetT;	//最近交点坐标
		glm::vec3 normal = glm::normalize(hitPoint);				//交点法线

		glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));	//光照方向
		float d = glm::max(glm::dot(normal, -lightDir), 0.0f);		//光照强度[0, 1]

		glm::vec3 sphereColor(1, 0, 1);
		sphereColor *= d;
		return glm::vec4(sphereColor, 1);
	}
}