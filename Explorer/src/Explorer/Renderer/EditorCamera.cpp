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
		UpdateView();	//更新视图矩阵
	}

	void EditorCamera::UpdateProjection()
	{
		m_AspectRatio = m_ViewportWidth / m_ViewportHeight;
		m_ProjectionMatrix = glm::perspective(glm::radians(m_FOV), m_AspectRatio, m_Near, m_Far);	//计算透视投影矩阵
	}

	void EditorCamera::UpdateView()
	{
		m_Position = CalculatePosition();			//计算相机位置

		glm::quat orientation = GetOrientation();	//计算相机方向
		//计算视图矩阵
		m_ViewMatrix = glm::translate(glm::mat4(1.0f), m_Position) * glm::toMat4(orientation);	//相机Transform矩阵
		m_ViewMatrix = glm::inverse(m_ViewMatrix);	//Transform的逆矩阵
	}

	std::pair<float, float> EditorCamera::PanSpeed() const
	{
		float x = std::min(m_ViewportWidth / 1000.0f, 2.4f);		//x最大移动速度2.4f
		float xFactor = 0.0366f * (x * x) - 0.1778f * x + 0.3021f;

		float y = std::min(m_ViewportHeight / 1000.0f, 2.4f);		//y最大移动速度2.4f
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
		distance = std::max(distance, 0.0f);	//最小缩放速度 = 0
		float speed = distance * distance;
		speed = std::min(speed, 100.0f);		//最大缩放速度 = 100
		return speed;
	}

	void EditorCamera::OnUpdate(DeltaTime dt)
	{
		const glm::vec2& mouse{ Input::GetMouseX(), Input::GetMouseY() };	//当前鼠标位置
		glm::vec2 delta = (mouse - m_InitialMousePosition) * 0.003f;		//鼠标移动增量 = 当前位置 - 初始位置
		m_InitialMousePosition = mouse;										//初始鼠标位置

		if (Input::IsKeyPressed(Key::LeftShift)){						//按下Shift键
			if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle)) {		//按下鼠标中键
				ViewPan(delta);	//视图移动
			}
		}
		else if (Input::IsMouseButtonPressed(Mouse::ButtonMiddle)) {	//不按Shift 按下鼠标中键
			ViewRotate(delta);	//视图旋转
		}

		UpdateView();	//更新视图矩阵
	}

	void EditorCamera::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);
		dispatcher.Dispatch<MouseScrolledEvent>(EXP_BIND_EVENT_FUNC(EditorCamera::OnMouseScroll));	//鼠标滚轮事件
	}

	bool EditorCamera::OnMouseScroll(MouseScrolledEvent& e)
	{
		float delta = e.GetYOffset() * 0.1f;	//滚轮Y偏移量
		ViewZoom(delta);						//视图缩放
		UpdateView();							//更新视图
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
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 1.0f, 0.0f));	//相机up y+
	}

	glm::vec3 EditorCamera::GetRightDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(1.0f, 0.0f, 0.0f));	//相机rigth x+
	}

	glm::vec3 EditorCamera::GetForwardDirection() const
	{
		return glm::rotate(GetOrientation(), glm::vec3(0.0f, 0.0f, -1.0f));	//相机forward z-
	}

	glm::vec3 EditorCamera::CalculatePosition() const
	{
		return m_FocalPoint - GetForwardDirection() * m_Distance;	//相机位置 = 焦点 - 相机到焦点到距离
	}

	glm::quat EditorCamera::GetOrientation() const
	{
		return glm::quat(glm::vec3(-m_Pitch, -m_Yaw, 0.0f));
	}
}
