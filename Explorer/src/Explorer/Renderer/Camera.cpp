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
		//�任���� = ƽ�ƾ��� * ��ת����- ��ת���� ��ת��z��
		glm::mat4 transform = glm::translate(glm::mat4(1.0f), m_Position) * glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation), glm::vec3(0, 0, 1.0f));

		m_ViewMatrix = glm::inverse(transform);						//����任 �� �����
		m_ViewProjectionMatrix = m_ProjectionMatrix * m_ViewMatrix;	//vp = p * v;
	}
}