#pragma once

#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// 相机
	/// </summary>
	class Camera
	{
	private:
		glm::mat4 m_ProjectionMatrix;		//投影矩阵
		glm::mat4 m_ViewMatrix;				//视图矩阵（相机变换的逆矩阵）
		glm::mat4 m_ViewProjectionMatrix;	//视图投影矩阵：p * v

		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };	//位置
		float m_Rotation = 0.0f;					//旋转（z轴）

		/// <summary>
		/// 更新视图矩阵
		/// </summary>
		void RecalculateViewMatrix();
	public:
		/// <summary>
		/// 正交相机
		/// </summary>
		/// <param name="left">左边界</param>
		/// <param name="right">右边界</param>
		/// <param name="bottom">下边界</param>
		/// <param name="top">上边界</param>
		Camera(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }

		/// <summary>
		/// 设置相机位置
		/// </summary>
		/// <param name="position">位置</param>
		void SetPosition(const glm::vec3& position)
		{
			m_Position = position;
			RecalculateViewMatrix();
		}

		float GetRotation() const { return m_Rotation; }

		/// <summary>
		/// 设置相机旋转（z轴）
		/// </summary>
		/// <param name="rotation">旋转角度（z轴）</param>
		void SetRotation(float rotation)
		{
			m_Rotation = rotation;
			RecalculateViewMatrix();
		}

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	};
}