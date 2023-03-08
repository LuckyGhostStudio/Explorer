#include "exppch.h"
#include "CameraController.h"

#include "Explorer/Core/Input.h"
#include "Explorer/Core/KeyCodes.h"

namespace Explorer
{
	CameraController::CameraController(float aspectRatio)
		//:m_AspectRatio(aspectRatio), 
		//m_Camera(-m_AspectRatio * m_ZoomLevel, m_AspectRatio* m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel)
	{

	}

	CameraController::CameraController(float fov, float aspectRatio, const glm::vec2& clippingPlane)
		//:m_AspectRatio(aspectRatio),
		//m_Camera(fov, m_AspectRatio, clippingPlane)
	{

	}

	void CameraController::OnUpdate(DeltaTime dt)
	{
		if (m_Camera.GetProjectionType() == Camera::ProjectionType::Orthographic) {	//�������
			//����ƶ�
			if (Input::IsKeyPressed(Key::A)) {		//��
				m_CameraPosition.x -= m_CameraTranslationSpeed * dt;
			}
			else if (Input::IsKeyPressed(Key::D)) {	//��
				m_CameraPosition.x += m_CameraTranslationSpeed * dt;
			}
			if (Input::IsKeyPressed(Key::W)) {		//��
				m_CameraPosition.y += m_CameraTranslationSpeed * dt;
			}
			else if (Input::IsKeyPressed(Key::S)) {	//��
				m_CameraPosition.y -= m_CameraTranslationSpeed * dt;
			}

			//m_Camera.SetPosition(m_CameraPosition);			//�������λ��

			m_CameraTranslationSpeed = m_ZoomLevel;		//�ƶ��ٶȺ����ű����������
		}
	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);	//�¼�������

		dispatcher.Dispatch<MouseScrolledEvent>(EXP_BIND_EVENT_FUNC(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(EXP_BIND_EVENT_FUNC(CameraController::OnWindowResized));
	}

	void CameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;	//���ÿ�߱�
		
		//m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);	//����ͶӰ����
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.5f;		//Y�����ű�����С
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);	//����>=0.25
		//m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);	//����ͶӰ����
		//TODO:͸��
		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());	//��߱�Ϊ ���� ��߱�
		
		return false;
	}
}