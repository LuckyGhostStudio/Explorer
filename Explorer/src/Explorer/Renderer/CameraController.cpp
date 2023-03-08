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
		if (m_Camera.GetProjectionType() == Camera::ProjectionType::Orthographic) {	//正交相机
			//相机移动
			if (Input::IsKeyPressed(Key::A)) {		//左
				m_CameraPosition.x -= m_CameraTranslationSpeed * dt;
			}
			else if (Input::IsKeyPressed(Key::D)) {	//右
				m_CameraPosition.x += m_CameraTranslationSpeed * dt;
			}
			if (Input::IsKeyPressed(Key::W)) {		//上
				m_CameraPosition.y += m_CameraTranslationSpeed * dt;
			}
			else if (Input::IsKeyPressed(Key::S)) {	//下
				m_CameraPosition.y -= m_CameraTranslationSpeed * dt;
			}

			//m_Camera.SetPosition(m_CameraPosition);			//设置相机位置

			m_CameraTranslationSpeed = m_ZoomLevel;		//移动速度和缩放比例线性相关
		}
	}

	void CameraController::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);	//事件调度器

		dispatcher.Dispatch<MouseScrolledEvent>(EXP_BIND_EVENT_FUNC(CameraController::OnMouseScrolled));
		dispatcher.Dispatch<WindowResizeEvent>(EXP_BIND_EVENT_FUNC(CameraController::OnWindowResized));
	}

	void CameraController::OnResize(float width, float height)
	{
		m_AspectRatio = width / height;	//设置宽高比
		
		//m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);	//设置投影矩阵
	}

	bool CameraController::OnMouseScrolled(MouseScrolledEvent& e)
	{
		m_ZoomLevel -= e.GetYOffset() * 0.5f;		//Y轴缩放比例减小
		m_ZoomLevel = std::max(m_ZoomLevel, 0.25f);	//缩放>=0.25
		//m_Camera.SetProjection(-m_AspectRatio * m_ZoomLevel, m_AspectRatio * m_ZoomLevel, -m_ZoomLevel, m_ZoomLevel);	//设置投影矩阵
		//TODO:透视
		return false;
	}

	bool CameraController::OnWindowResized(WindowResizeEvent& e)
	{
		OnResize((float)e.GetWidth(), (float)e.GetHeight());	//宽高比为 窗口 宽高比
		
		return false;
	}
}