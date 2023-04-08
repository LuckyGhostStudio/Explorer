#pragma once

#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// ��Դ���
	/// </summary>
	class Light
	{
	public:
		/// <summary>
		/// ��Դ����
		/// </summary>
		enum class Type
		{
			Directional = 0,	//ƽ�й�
			Point = 1,			//���Դ
			Spot = 2			//�۹�
		};
		
	private:
		Type m_Type = Type::Point;					//��Դ����

		float m_Range = 100.0f;						//���հ뾶��Point��Spot��������ǿ��˥��Ϊ0ʱ�ľ���
		glm::vec3 m_Color = { 1.0f, 1.0f, 1.0f };	//��ɫ

		float m_SpotOuterAngle = glm::radians(20.0f);	//Spot���Ž� ���ȣ���Ӱ���Ե��[1, 179]
		float m_SpotInnerAngle = glm::radians(18.3f);	//Spot���Ž� ���ȣ���Ӱ�ڱ�Ե��= Outer - Outer / 12

		float m_Intensity = 1.0f;					//����ǿ��
		
		bool m_CastShadow = true;					//�Ƿ�Ͷ����Ӱ
	public:
		Light() = default;
		Light(Type type) :m_Type(type) {}
		Light(const Light&) = default;

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