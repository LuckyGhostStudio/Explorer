#pragma once

#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// 光源组件
	/// </summary>
	class Light
	{
	public:
		/// <summary>
		/// 光源类型
		/// </summary>
		enum class Type
		{
			Directional = 0,	//平行光
			Point = 1,			//点光源
			Spot = 2			//聚光
		};
		
		glm::vec3 m_Color = { 1.0f, 1.0f, 1.0f };	//颜色
		bool m_RenderShadow = true;					//是否渲染阴影
	private:
		Type m_Type = Type::Point;					//光源类型

		float m_Range = 100.0f;						//光照半径（Point和Spot）：光照强度衰减为0时的距离

		float m_SpotOuterAngle = 20.0f;				//Spot外张角（阴影外边缘）[1, 179]
		float m_SpotInnerAngle = 18.3f;				//Spot内张角（阴影内边缘）= Outer - Outer / 12

		float m_Intensity = 1.0f;					//光照强度
	public:
		Light() = default;
		Light(Type type) :m_Type(type) {}
		Light(const Light&) = default;

		Type GetType() const { return m_Type; }
		void SetType(Type type) { m_Type = type; }

		float GetRange() const { return m_Range; }
		void SetRange(float range) { m_Range = range; }

		float GetSpotOuterAngle() const { return m_SpotOuterAngle; }
		void SetSpotOuterAngle(float spotOuterAngle) { m_SpotOuterAngle = spotOuterAngle; }

		float GetSpotInnerAngle() const { return m_SpotInnerAngle; }
		void SetSpotInnerAngle(float spotInnerAngle) { m_SpotInnerAngle = spotInnerAngle; }

		float GetIntensity() const { return m_Intensity; }
		void SetIntensity(float intensity) { m_Intensity = intensity; }
	};
}