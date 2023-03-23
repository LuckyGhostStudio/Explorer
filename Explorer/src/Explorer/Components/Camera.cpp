#include "exppch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Explorer
{
	Camera::Camera() 
	{
		RecalculateProjection();	//���¼���ͶӰ����
	}

	void Camera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Orthographic;	//����ͶӰ

		m_Size = size;
		m_Near = nearClip;
		m_Far = farClip;

		RecalculateProjection();	//���¼���ͶӰ����
	}

	void Camera::SetPerspective(float fov, float nearClip, float farClip)
	{
		m_ProjectionType = ProjectionType::Perspective;		//͸��ͶӰ

		m_Fov = fov;
		m_Near = nearClip;
		m_Far = farClip;

		RecalculateProjection();	//���¼���ͶӰ����
	}

	void Camera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;	//���߱�
		RecalculateProjection();	//���¼���ͶӰ����
	}

	void Camera::RecalculateProjection()
	{
		if (m_ProjectionType == ProjectionType::Perspective) {	//͸��ͶӰ
			m_Projection = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_Near, m_Far);	//����͸��ͶӰ����
		}
		else {
			float orthLeft = -m_Size * m_AspectRatio;	//��߽�
			float orthRight = m_Size * m_AspectRatio;	//�ұ߽�
			float orthBottom = -m_Size;					//�±߽�
			float orthTop = m_Size;						//�ϱ߽�

			m_Projection = glm::ortho(orthLeft, orthRight, orthBottom, orthTop, m_Near, m_Far);	//��������ͶӰ����
		}
	}
}