#include "exppch.h"
#include "CameraController.h"

#include "Explorer/Core/Input.h"
#include "Explorer/Core/KeyCodes.h"

namespace Explorer
{
	CameraController::CameraController(float aspectRatio, bool rotation)
		:m_AspectRatio(aspectRatio), 
		m_Bounds({ -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel }), 
		m_Camera(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top), 
		m_Rotation(rotation)
	{

	}

	void CameraController::OnUpdate(DeltaTime dt)
	{
		//����ƶ�
		if (Input::IsKeyPressed(EXP_KEY_A)) {		//��
			m_CameraPosition.x -= m_CameraTranslationSpeed * dt;
		}
		else if (Input::IsKeyPressed(EXP_KEY_D)) {	//��
			m_CameraPosition.x += m_CameraTranslationSpeed * dt;
		}
		if (Input::IsKeyPressed(EXP_KEY_W)) {		//��
			m_CameraPosition.y += m_CameraTranslationSpeed * dt;
		}
		else if (Input::IsKeyPressed(EXP_KEY_S)) {	//��
			m_CameraPosition.y -= m_CameraTranslationSpeed * dt;
		}

		if (m_Rotation) {
			if (Input::IsKeyPressed(EXP_KEY_Q)) {
				m_CameraRotation += m_CameraRotationSpeed * dt;
			}
			if (Input::IsKeyPressed(EXP_KEY_E)) {
				m_CameraRotation -= m_CameraRotationSpeed * dt;
			}

			m_Camera.SetRotation(m_CameraRotation);		//���������ת
		}

		m_Camera.SetPosition(m_CameraPosition);			//�������λ��

		m_CameraTranslationSpeed = m_ZoomLevel;		//�ƶ��ٶȺ����ű����������
	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);	//�¼�������

		dispatcher.Dispatch<MouseScrolledEvent>(EXP_BIND_EVENT_FUNC(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(EXP_BIND_EVENT_FUNC(CameraController::OnWindowResized));
	}

	void CameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };//��������߽�
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);				//����ͶӰ����
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.5f;		//Y�����ű�����С
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);	//����>=0.25
		CalculateView();							//������ͼ

		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();	//��߱�Ϊ ���� ��߱�
		CalculateView();											//������ͼ

		return false;
	}
}