#pragma once

#include <glm/glm.hpp>

#include "Explorer/Components/Component.h"

namespace Explorer
{
	/// <summary>
	/// 相机组件
	/// </summary>
	class Camera :public Component
	{
	public:
		/// <summary>
		/// 清屏标志
		/// </summary>
		enum class ClearFlag { Color = 0, Skybox = 1 };

		/// <summary>
		/// 投影类型
		/// </summary>
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	private:
		bool m_Primary = true;	//是主相机

		ClearFlag m_ClearFlag = ClearFlag::Color;						//清屏类型
		glm::vec4 m_BackgroundColor = { 0.2f, 0.3f, 0.5f, 1.0f };		//背景颜色：清屏颜色
		ProjectionType m_ProjectionType = ProjectionType::Perspective;	//投影类型

		float m_Fov = glm::radians(45.0f);	//透视相机张角：垂直方向（弧度）
		float m_Size = 5.0f;				//正交相机尺寸：垂直方向一半长度
		
		float m_Near = 0.01f;				//近裁剪平面
		float m_Far = 1000.0f;				//远裁剪平面

		float m_AspectRatio = 0.0f;			//屏幕宽高比（X/Y）

		glm::mat4 m_Projection;				//投影矩阵
	private:
		/// <summary>
		/// 重新计算投影矩阵
		/// </summary>
		void RecalculateProjection();

		virtual void SetName() override { m_Name = "Camera"; }

		/// <summary>
		/// 设置Camera组件图标
		/// </summary>
		virtual void SetIcon() override { m_Icon = std::make_shared<Texture2D>("asserts/textures/defaults/Icons/Components/Camera_Icon.png"); }
	public:
		Camera();

		virtual const std::string& GetName() override { SetName(); return m_Name; }
		virtual std::shared_ptr<Texture2D>& GetIcon() override { SetIcon(); return m_Icon; }

		/// <summary>
		/// 设置正交投影参数->计算投影矩阵
		/// </summary>
		/// <param name="fov">垂直张角大小</param>
		/// <param name="nearClip">近裁剪平面</param>
		/// <param name="farClip">远裁剪平面</param>
		void SetPerspective(float fov, float nearClip, float farClip);

		/// <summary>
		/// 设置正交投影参数->计算投影矩阵
		/// </summary>
		/// <param name="size">尺寸：垂直方向</param>
		/// <param name="nearClip">近裁剪平面</param>
		/// <param name="farClip">远裁剪平面</param>
		void SetOrthographic(float size, float nearClip, float farClip);

		/// <summary>
		/// 设置视口大小->计算投影矩阵
		/// </summary>
		/// <param name="width">宽</param>
		/// <param name="height">高</param>
		void SetViewportSize(uint32_t width, uint32_t height);

		const glm::mat4& GetProjection() const { return m_Projection; }
		
		bool IsPrimary() const { return m_Primary; }
		bool& IsPrimary_Ref() { return m_Primary; }
		void SetPrimary(bool primary) { m_Primary = primary; }

		ClearFlag GetClearFlag() const { return m_ClearFlag; }
		void SetClearFlag(const ClearFlag flag) { m_ClearFlag = flag; }

		glm::vec4& GetBackgroundColor() { return m_BackgroundColor; }
		void SetBackgroundColor(const glm::vec4& color) { m_BackgroundColor = color; }

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

		float GetNearClip() const { return m_Near; }
		float& GetNearClip_Ref() { return m_Near; }
		void SetNearClip(float nearClip) { m_Near = nearClip; RecalculateProjection(); }

		float GetFarClip() const { return m_Far; }
		float& GetFarClip_Ref() { return m_Far; }
		void SetFarClip(float farClip) { m_Far = farClip; RecalculateProjection(); }

		float GetSize() const { return m_Size; }
		float& GetSize_Ref() { return m_Size; }
		void SetSize(float size) { m_Size = size; RecalculateProjection(); }

		float GetFOV() const { return m_Fov; }
		float& GetFOV_Ref() { return m_Fov; }
		void SetFOV(float fov) { m_Fov = fov; RecalculateProjection(); }
	};
}