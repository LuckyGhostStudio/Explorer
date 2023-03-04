#include "exppch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Explorer
{
	Camera::Camera() 
		:Component("Camera"),
		m_ClearFlag(ClearFlag::Color), m_BackgroundColor({ 0.1f, 0.1f, 0.1f, 1.0f }), m_Type(CameraType::Orthographic)
	{
		RecalculateProjection();	//重新计算投影矩阵
	}

	void Camera::SetOrthographic(float size, float nearClip, float farClip)
	{
		m_Size = size;
		m_Near = nearClip;
		m_Far = farClip;

		RecalculateProjection();	//重新计算投影矩阵
	}

	void Camera::SetPerspective(float fov, float aspectRatio, float nearClip, float farClip)
	{
		m_Fov = fov;
		m_AspectRatio = aspectRatio;
		m_Near = nearClip;
		m_Far = farClip;

		RecalculateProjection();	//重新计算投影矩阵
	}

	void Camera::SetViewportSize(uint32_t width, uint32_t height)
	{
		m_AspectRatio = (float)width / (float)height;	//宽高比
		RecalculateProjection();	//重新计算投影矩阵
	}

	void Camera::RecalculateProjection()
	{
		if (m_Type == CameraType::Orthographic) {		//正交相机
			float orthLeft = -m_Size * m_AspectRatio;	//左边界
			float orthRight = m_Size * m_AspectRatio;	//右边界
			float orthBottom = -m_Size;					//下边界
			float orthTop = m_Size;						//上边界

			m_Projection = glm::ortho(orthLeft, orthRight, orthBottom, orthTop, m_Near, m_Far);	//正交投影矩阵
		}
		else {	//透视相机
			m_Projection = glm::perspective(glm::radians(m_Fov), m_AspectRatio, m_Near, m_Far);	//透视投影矩阵
		}
	}
}