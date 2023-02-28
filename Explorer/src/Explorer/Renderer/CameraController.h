#pragma once

#include "Explorer/Core/DeltaTime.h"
#include "Explorer/Renderer/Camera.h"

#include "Explorer/Events/ApplicationEvent.h"
#include "Explorer/Events/MouseEvent.h"

namespace Explorer
{
	/// <summary>
	/// 相机边界
	/// </summary>
	struct CameraBounds
	{
		float Left, Right;
		float Bottom, Top;

		float GetWidth() { return Right - Left; }
		float GetHeight() { return Top - Bottom; }
	};

	/// <summary>
	/// 相机控制器
	/// </summary>
	class CameraController
	{
	private:
		float m_AspectRatio;		//宽高比（X/Y）
		float m_ZoomLevel = 1.0f;	//缩放比例（Y）
		CameraBounds m_Bounds;		//相机边界
		Camera m_Camera;			//相机

		bool m_Rotation;			//是否可旋转

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };	//相机位置
		float m_CameraRotation = 0.0f;						//相机旋转（z轴）
		float m_CameraTranslationSpeed = 2.0f;				//相机移动速度
		float m_CameraRotationSpeed = 90.0f;				//相机旋转速度
	public:
		/// <summary>
		/// 正交相机控制器
		/// </summary>
		/// <param name="aspectRatio">宽高比（X/Y）</param>
		/// <param name="rotation">是否可旋转（false）</param>
		CameraController(float aspectRatio, bool rotation = false);

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

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }

		/// <summary>
		/// 设置缩放比例
		/// </summary>
		/// <param name="level">缩放比例</param>
		void SetZoomLevel(float level)
		{
			m_ZoomLevel = level; 
			CalculateView();
		}
	private:
		/// <summary>
		/// 计算视图大小
		/// </summary>
		void CalculateView();

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