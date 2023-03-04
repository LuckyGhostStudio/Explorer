#pragma once

#include "Explorer/Core/DeltaTime.h"
#include "Explorer/Renderer/Camera.h"

#include "Explorer/Events/ApplicationEvent.h"
#include "Explorer/Events/MouseEvent.h"

namespace Explorer
{
	/// <summary>
	/// 相机控制器
	/// </summary>
	class CameraController
	{
	private:
		float m_AspectRatio;		//宽高比（X/Y）
		float m_ZoomLevel = 1.0f;	//缩放比例（Y）
		Camera m_Camera;			//相机

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };	//相机位置
		glm::vec3 m_CameraRotation = { 0.0f, 0.0f, 0.0f };	//相机旋转
		float m_CameraTranslationSpeed = 2.0f;				//相机移动速度
		float m_CameraRotationSpeed = 90.0f;				//相机旋转速度
	public:
		/// <summary>
		/// 正交相机控制器
		/// </summary>
		/// <param name="aspectRatio">宽高比（X/Y）</param>
		CameraController(float aspectRatio);

		/// <summary>
		/// 透视相机控制器
		/// </summary>
		/// <param name="fov">垂直方向张角</param>
		/// <param name="aspectRatio">屏幕宽高比</param>
		/// <param name="clippingPlane">裁剪平面（近，远）</param>
		CameraController(float fov, float aspectRatio, const glm::vec2& clippingPlane);

		/// <summary>
		/// 更新：每帧调用
		/// </summary>
		/// <param name="dt">帧间隔</param>
		void OnUpdate(DeltaTime dt);

		/// <summary>
		/// 事件回调函数：事件发生时调用
		/// </summary>
		/// <param name="e">事件</param>
		void OnEvent(Event& e);

		/// <summary>
		/// 重置相机大小
		/// </summary>
		/// <param name="width">宽</param>
		/// <param name="height">高</param>
		void OnResize(float width, float height);

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }

		/// <summary>
		/// 设置缩放比例
		/// </summary>
		/// <param name="level">缩放比例</param>
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		/// <summary>
		/// 鼠标滚轮滚动
		/// </summary>
		/// <param name="e">鼠标滚轮滚动事件</param>
		/// <returns>事件处理结果</returns>
		bool OnMouseScrolled(MouseScrolledEvent& e);

		/// <summary>
		/// 窗口缩放
		/// </summary>
		/// <param name="e">窗口缩放事件</param>
		/// <returns>事件处理结果</returns>
		bool OnWindowResized(WindowResizeEvent& e);
	};
}