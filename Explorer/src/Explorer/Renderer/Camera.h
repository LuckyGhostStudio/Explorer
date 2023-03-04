#pragma once

#include <glm/glm.hpp>

#include "Explorer/Scene/Components.h"

namespace Explorer
{
	/// <summary>
	/// 清屏标志
	/// </summary>
	enum class ClearFlag
	{
		Color = 0,	//颜色
		Skybox = 1	//天空盒
	};

	/// <summary>
	/// 相机类型
	/// </summary>
	enum class CameraType
	{
		Orthographic = 0,	//正交
		Perspective = 1		//透视
	};

	/// <summary>
	/// 相机
	/// </summary>
	class Camera :public Component
	{
	public:
		//Temp
		bool m_Primary = true;	//是主相机
	private:
		ClearFlag m_ClearFlag;			//清屏类型
		glm::vec4 m_BackgroundColor;	//背景颜色：清屏颜色
		CameraType m_Type;				//相机类型
		glm::mat4 m_Projection;			//投影矩阵
		float m_AspectRatio = 0.0f;		//屏幕宽高比（X/Y）
		float m_Near = -1.0f;			//近裁剪平面
		float m_Far = 1.0f;				//远裁剪平面

		float m_Size = 5.0f;			//正交相机尺寸：垂直方向一半长度
		float m_Fov = 45.0f;			//透视相机垂直方向张角
	private:
		/// <summary>
		/// 重新计算投影矩阵
		/// </summary>
		void RecalculateProjection();
	public:
		Camera();

		/// <summary>
		/// 设置正交投影参数->计算投影矩阵
		/// </summary>
		/// <param name="size">尺寸：垂直方向</param>
		/// <param name="nearClip">近裁剪平面</param>
		/// <param name="farClip">远裁剪平面</param>
		void SetOrthographic(float size, float nearClip, float farClip);

		/// <summary>
		/// 设置正交投影参数->计算投影矩阵
		/// </summary>
		/// <param name="fov">垂直张角大小</param>
		/// <param name="aspectRatio">宽高比</param>
		/// <param name="nearClip">近裁剪平面</param>
		/// <param name="farClip">远裁剪平面</param>
		void SetPerspective(float fov, float aspectRatio, float nearClip, float farClip);

		/// <summary>
		/// 设置视口大小->计算投影矩阵
		/// </summary>
		/// <param name="width">宽</param>
		/// <param name="height">高</param>
		void SetViewportSize(uint32_t width, uint32_t height);

		ClearFlag GetClearFlag() const { return m_ClearFlag; }
		void SetClearFlag(const ClearFlag& flag) { m_ClearFlag = flag; }

		const glm::vec4& GetBackgroundColor() const { return m_BackgroundColor; }
		void SetBackgroundColor(const glm::vec4& background) { m_BackgroundColor = background; }

		CameraType GetType() const { return m_Type; }
		void SetType(const CameraType& type) { m_Type = type; RecalculateProjection(); }

		const glm::mat4& GetProjection() const { return m_Projection; }

		float GetNearClip() const { return m_Near; }
		void SetNearClip(float nearClip) { m_Near = nearClip; RecalculateProjection(); }

		float GetFarClip() const { return m_Far; }
		void SetFarClip(float farClip) { m_Far = farClip; RecalculateProjection(); }

		float GetSize() const { return m_Size; }
		void SetSize(float size) { m_Size = size; RecalculateProjection(); }

		float GetFOV() const { return m_Fov; }
		void SetFOV(float fov) { m_Fov = fov; RecalculateProjection(); }
	};
}