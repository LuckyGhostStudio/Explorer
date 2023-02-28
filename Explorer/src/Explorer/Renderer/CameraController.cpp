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
		//相机移动
		if (Input::IsKeyPressed(EXP_KEY_A)) {		//左
			m_CameraPosition.x -= m_CameraTranslationSpeed * dt;
		}
		else if (Input::IsKeyPressed(EXP_KEY_D)) {	//右
			m_CameraPosition.x += m_CameraTranslationSpeed * dt;
		}
		if (Input::IsKeyPressed(EXP_KEY_W)) {		//上
			m_CameraPosition.y += m_CameraTranslationSpeed * dt;
		}
		else if (Input::IsKeyPressed(EXP_KEY_S)) {	//下
			m_CameraPosition.y -= m_CameraTranslationSpeed * dt;
		}

		if (m_Rotation) {
			if (Input::IsKeyPressed(EXP_KEY_Q)) {
				m_CameraRotation += m_CameraRotationSpeed * dt;
			}
			if (Input::IsKeyPressed(EXP_KEY_E)) {
				m_CameraRotation -= m_CameraRotationSpeed * dt;
			}

			m_Camera.SetRotation(m_CameraRotation);		//设置相机旋转
		}

		m_Camera.SetPosition(m_CameraPosition);			//设置相机位置

		m_CameraTranslationSpeed = m_ZoomLevel;		//移动速度和缩放比例线性相关
	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);	//事件调度器

		dispatcher.Dispatch<MouseScrolledEvent>(EXP_BIND_EVENT_FUNC(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(EXP_BIND_EVENT_FUNC(CameraController::OnWindowResized));
	}

	void CameraController::CalculateView()
	{
		m_Bounds = { -m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel };//设置相机边界
		m_Camera.SetProjection(m_Bounds.Left, m_Bounds.Right, m_Bounds.Bottom, m_Bounds.Top);				//设置投影矩阵
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.5f;		//Y轴缩放比例减小
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);	//缩放>=0.25
		CalculateView();							//计算视图

		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		m_AspectRatio = (float)e.GetWidth() / (float)e.GetHeight();	//宽高比为 窗口 宽高比
		CalculateView();											//计算视图

		return false;
	}
}