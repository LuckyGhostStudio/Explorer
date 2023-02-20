#include "exppch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Explorer
{
	Camera::Camera(float left, float right, float bottom, float top)
		:m_ProjectionMatrix(glm::ortho(left, right, bottom, top, -1.0f, 1.0f)), m_ViewMatrix(1.0f)
	{
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;	//vp = p * v;
	}

	void Camera::RecalculateViewMatrix()
	{
		//变换矩阵 = 平移矩阵 * 旋转矩阵（- 旋转弧度 旋转轴z）
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1.0f));

		m_ViewMatrix = glm::inverse(transform);						//相机变换 求 逆矩阵
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;	//vp = p * v;
	}
}