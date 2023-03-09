#include "exppch.h"
#include "EditorCamera.h"

#include "Explorer/Core/Input.h"
#include "Explorer/Core/KeyCodes.h"
#include "Explorer/Core/MouseButtonCodes.h"

#include <glfw/glfw3.h>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Explorer 
{
	EditorCamera::EditorCamera(float fov, float aspectRatio, float nearClip, float farClip)
		: m_FOV(fov), m_AspectRatio(aspectRatio), m_Near(nearClip), m_Far(farClip)
	{
		UpdateView();	//������ͼ����
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);	//����͸��ͶӰ����
	}

	void EditorCamera::UpdateView()
	{
		m_Position = CalculatePosition();			//�������λ��

		glm::quat orientation = GetOrientation();	//�����������
		//������ͼ����
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);	//���Transform����
		m_ViewMatrix = glm::inverse(m_ViewMatrix);	//Transform�������
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f);		//x����ƶ��ٶ�2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f);		//y����ƶ��ٶ�2.4f
		float yFactor = 0.0366f * (y * y) - 0.1778f * y + 0.3021f;

		return { xFactor, yFactor };
	}

	float EditorCamera::RotationSpeed() const
	{
		return 0.8f;
	}

	float EditorCamera::ZoomSpeed() const
	{
		float distance = m_Distance * 0.2f;
		distance = std::max(distance, 0.0f);	//��С�����ٶ� = 0
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);		//��������ٶ� = 100
		return speed;
	}

	void EditorCamera::OnUpdate(DeltaTime dt)
	{
		const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };	//��ǰ���λ��
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;		//����ƶ����� = ��ǰλ�� - ��ʼλ��
		m_InitialMousePosition = mouse;										//��ʼ���λ��

		if (Input::IsKeyPressed(Key::LeftShift)){						//����Shift��
			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle)) {		//��������м�
				ViewPan(delta);	//��ͼ�ƶ�
			}
		}
		else if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle)) {	//����Shift ��������м�
			ViewRotate(delta);	//��ͼ��ת
		}

		UpdateView();	//������ͼ����
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(EXP_BIND_EVENT_FUNC(EditorCamera::OnMouseScroll));	//�������¼�
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;	//����Yƫ����
		ViewZoom(delta);						//��ͼ����
		UpdateView();							//������ͼ
		return false;
	}

	void EditorCamera::ViewPan(const glm::vec2& delta)
	{
		auto [xSpeed, ySpeed] = PanSpeed();
		m_FocalPoint += -GetRightDirection() * delta.x * xSpeed * m_Distance;
		m_FocalPoint += GetUpDirection() * delta.y * ySpeed * m_Distance;
	}

	void EditorCamera::ViewRotate(const glm::vec2& delta)
	{
		float yawSign = GetUpDirection().y < 0 ? -1.0f : 1.0f;
		m_Yaw += yawSign * delta.x * RotationSpeed();
		m_Pitch += delta.y * RotationSpeed();
	}

	void EditorCamera::ViewZoom(float delta)
	{
		m_Distance -= delta * ZoomSpeed();
		if (m_Distance < 1.0f)
		{
			m_FocalPoint += GetForwardDirection();
			m_Distance = 1.0f;
		}
	}

	glm::vec3 EditorCamera::GetUpDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));	//���up y+
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));	//���rigth x+
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));	//���forward z-
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;	//���λ�� = ���� - ��������㵽����
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}
}
