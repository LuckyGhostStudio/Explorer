#pragma once

#include <glm/glm.hpp>

#include "Explorer/Components/Component.h"
#include "Explorer/Renderer/Shader.h"

namespace Explorer
{
	/// <summary>
	/// 光源组件
	/// </summary>
	class Light	:public Component
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
		
	private:
		Type m_Type = Type::Point;					//光源类型

		float m_Range = 5.0f;						//光照半径（Point和Spot）：光照强度衰减为0时的距离
		glm::vec3 m_Color = { 1.0f, 1.0f, 1.0f };	//颜色

		float m_SpotOuterAngle = glm::radians(20.0f);	//Spot外张角 弧度（阴影外边缘）[1, 179]
		float m_SpotInnerAngle = glm::radians(18.3f);	//Spot内张角 弧度（阴影内边缘）= Outer - Outer / 12

		float m_Intensity = 20.0f;					//光照强度
		bool m_CastShadow = true;					//是否投射阴影
	private:
		virtual void SetName() override { m_Name = "Light"; }

		//设置Light组件图标：根据Light类型设置不同图标
		virtual void SetIcon() override;
	public:
		Light() = default;
		Light(Type type) :m_Type(type) {}
		Light(const Light&) = default;

		virtual const std::string& GetName() override { SetName(); return m_Name; }
		virtual std::shared_ptr<Texture2D>& GetIcon() override { SetIcon(); return m_Icon; }

		/// <summary>
		/// 设置Shader Uniform数据
		/// </summary>
		void SetShaderData(const glm::vec3& position, const glm::vec3& direction, const std::shared_ptr<Shader>& shader, uint32_t index);

		Type GetType() const { return m_Type; }
		void SetType(Type type) { m_Type = type; }

		float GetRange() const { return m_Range; }
		float& GetRange_Ref() { return m_Range; }
		void SetRange(float range) { m_Range = range; }

		glm::vec3& GetColor() { return m_Color; }
		void SetColor(const glm::vec3& color) { m_Color = color; }

		float GetSpotOuterAngle() const { return m_SpotOuterAngle; }
		float& GetSpotOuterAngle_Ref() { return m_SpotOuterAngle; }
		void SetSpotOuterAngle(float spotOuterAngle) { m_SpotOuterAngle = spotOuterAngle; SetSpotInnerAngle(); }

		float GetSpotInnerAngle() const { return m_SpotInnerAngle; }
		void SetSpotInnerAngle(float spotInnerAngle) { m_SpotInnerAngle = spotInnerAngle; }
		void SetSpotInnerAngle() { m_SpotInnerAngle = m_SpotOuterAngle - m_SpotOuterAngle / 12.0f; }

		float GetIntensity() const { return m_Intensity; }
		float& GetIntensity_Ref() { return m_Intensity; }
		void SetIntensity(float intensity) { m_Intensity = intensity; }

		bool GetCastShadow() const { return m_CastShadow; }
		bool& GetCastShadow_Ref() { return m_CastShadow; }
		void SetCastShadow(bool cast) { m_CastShadow = cast; }
	};
}