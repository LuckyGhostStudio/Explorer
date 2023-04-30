#pragma once

#include "Explorer/Core/DeltaTime.h"
#include "Explorer/Events/Event.h"
#include "Explorer/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// 编辑器相机
	/// </summary>
	class EditorCamera
	{
	private:
		float m_FOV = glm::radians(60.0f);		//相机垂直张角
		float m_Near = 0.01f;					//近裁剪平面
		float m_Far = 1000.0f;					//远裁剪平面
		float m_AspectRatio = 1280.0f / 720.0f;	//屏幕宽高比

		glm::mat4 m_ViewMatrix;								//视图矩阵
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);		//投影矩阵

		glm::vec3 m_Position = { 14.6f, 10.0f, -14.0f };	//相机位置
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };		//焦点位置

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };	//鼠标初始位置

		float m_Distance = 10.0f;					//相机与焦点距离
		float m_Pitch = 0.3f;						//俯仰角（x轴）弧度
		float m_Yaw = -0.98f;						//偏航角（y轴）弧度

		float m_RotationSpeed = 0.8f;		//旋转速度
		float m_ViewZoomRate = 0.3f;		//视图缩放速率

		float m_ViewportWidth = 1280.0f;	//视口宽
		float m_ViewportHeight = 720;		//视口高
	private:
		/// <summary>
		/// 更新投影矩阵
		/// </summary>
		void UpdateProjection();

		/// <summary>
		/// 更新视图矩阵
		/// </summary>
		void UpdateView();

		/// <summary>
		/// 鼠标滚轮滚动时调用
		/// </summary>
		/// <param name="e">鼠标滚动事件</param>
		/// <returns>处理结果</returns>
		bool OnMouseScroll(MouseScrolledEvent& e);

		void ViewPan(const glm::vec2& delta);
		void ViewRotate(const glm::vec2& delta);
		void ViewZoom(float delta);

		/// <summary>
		/// 计算相机位置
		/// </summary>
		/// <returns></returns>
		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	public:
		EditorCamera() = default;

		/// <summary>
		/// 编辑器相机
		/// </summary>
		/// <param name="fov">垂直张角</param>
		/// <param name="aspectRatio">屏幕宽高比</param>
		/// <param name="nearClip">近裁剪平面</param>
		/// <param name="farClip">远裁剪平面</param>
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		/// <summary>
		/// 更新相机：每帧
		/// </summary>
		/// <param name="dt">帧间隔</param>
		void OnUpdate(DeltaTime dt);

		/// <summary>
		/// 事件回调函数
		/// </summary>
		/// <param name="e">事件</param>
		void OnEvent(Event& e);

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }
		
		float GetFOV() const { return m_FOV; }
		float& GetFOV_Ref() { UpdateProjection(); return m_FOV; }
		void SetFOV(float fov) { m_FOV = fov; UpdateProjection(); }

		float GetNearClip() const { return m_Near; }
		float& GetNearClip_Ref() { UpdateProjection(); return m_Near; }
		void SetNearClip(float nearClip) { m_Near = nearClip; UpdateProjection(); }

		float GetFarClip() const { return m_Far; }
		float& GetFarClip_Ref() { UpdateProjection(); return m_Far; }
		void SetFarClip(float farClip) { m_Far = farClip; UpdateProjection(); }

		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		glm::mat4 GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;

		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }

		float GetRotationSpeed() const { return m_RotationSpeed; }
		float& GetRotationSpeed_Ref() { return m_RotationSpeed; }

		float GetViewZoomRate() const { return m_ViewZoomRate; }
		float& GetViewZoomRate_Ref() { return m_ViewZoomRate; }
	};
}
