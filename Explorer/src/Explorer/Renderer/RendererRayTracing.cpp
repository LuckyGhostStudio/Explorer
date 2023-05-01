#include "exppch.h"
#include "RendererRayTracing.h"

#include "Explorer/Utils/Random.h"

namespace Explorer
{
	namespace Utils
	{
		/// <summary>
		/// vec4ת��Ϊ32λRBGA��16���ƣ�
		/// </summary>
		/// <param name="color">��ɫ</param>
		/// <returns>32λ16������ɫ</returns>
		static uint32_t ConvertToRGBA(const glm::vec4& color)
		{
			uint8_t r = (uint8_t)(color.r * 255.0f);	//��ɫͨ��
			uint8_t g = (uint8_t)(color.g * 255.0f);	//��ɫͨ��
			uint8_t b = (uint8_t)(color.b * 255.0f);	//��ɫͨ��
			uint8_t a = (uint8_t)(color.a * 255.0f);	//͸��ͨ��

			uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;	//ABGR

			return result;
		}
	}

	void RendererRayTracing::OnResize(uint32_t width, uint32_t height)
	{
		//ͼ�����
		if (m_FinalImage){
			if (m_FinalImage->GetWidth() == width && m_FinalImage->GetHeight() == height) return;

			//m_FinalImage->Resize(width, height, m_ImageData);	//���ô�С
			m_FinalImage = std::make_shared<Texture2D>(width, height);	//����ͼ��
		}
		else{	//������
			m_FinalImage = std::make_shared<Texture2D>(width, height);	//����ͼ��
		}

		delete[] m_ImageData;
		m_ImageData = new uint32_t[width * height];
	}

	void RendererRayTracing::Render()
	{
		float aspectRatio = m_FinalImage->GetWidth() / (float)m_FinalImage->GetHeight();	//��߱�

		//����ÿ������
		for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++){
			for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++){
				glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };	//����[0, 1]
				coord = coord * 2.0f - 1.0f;	//coord = [-1, 1] �������Ļ����(0, 0)λ��
				coord.x *= aspectRatio;

				glm::vec4 color = PerPixel(coord);	//coord�����������ɫ
				color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));	//����color = [0, 1]
				m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);	//ͼ������
			}
		}
		
		m_FinalImage->SetData(m_ImageData, m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(uint32_t));
	}

	glm::vec4 RendererRayTracing::PerPixel(glm::vec2 coord)
	{
		glm::vec3 rayOrigin(0.0f, 0.0f, 1.0f);				//����Դ ���λ��
		glm::vec3 rayDirection(coord.x, coord.y, -1.0f);	//���߷��򣺴������0, 0��ָ��ÿ����������
		float radius = 0.5f;								//��뾶

		float a = glm::dot(rayDirection, rayDirection);				//������ϵ����direction.x ^ 2 + direction.y ^ 2 + direction.z ^ 2
		float b = 2.0f * glm::dot(rayOrigin, rayDirection);			//һ����ϵ����2 * (origin.x * direction.x + origin.y * direction.y + origin.z * direction.z)
		float c = glm::dot(rayOrigin, rayOrigin) - radius * radius;	//�����origin.x ^ 2 + origin.y ^ 2 + origin.z ^ 2 - radius ^ 2

		float discriminant = b * b - 4.0f * a * c;		//�������򽻵��б�ʽ

		//�޽���
		if (discriminant < 0.0f) {
			return glm::vec4(0.1, 0.1, 0.1, 1);
		}

		float t0 = (-b + glm::sqrt(discriminant)) / 2.0f * a;		//Դ���뽻��0����
		float closetT = (-b - glm::sqrt(discriminant)) / 2.0f * a;	//Դ�����������ľ���

		glm::vec3 hitPoint = rayOrigin + rayDirection * closetT;	//�����������
		glm::vec3 normal = glm::normalize(hitPoint);				//���㷨��

		glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));	//���շ���
		float d = glm::max(glm::dot(normal, -lightDir), 0.0f);		//����ǿ��[0, 1]

		glm::vec3 sphereColor(1, 0, 1);
		sphereColor *= d;
		return glm::vec4(sphereColor, 1);
	}
}