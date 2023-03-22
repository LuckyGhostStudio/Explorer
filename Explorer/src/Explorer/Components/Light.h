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
		
		glm::vec3 m_Color = { 1.0f, 1.0f, 1.0f };	//��ɫ
		bool m_RenderShadow = true;					//�Ƿ���Ⱦ��Ӱ
	private:
		Type m_Type = Type::Point;					//��Դ����

		float m_Range = 100.0f;						//���հ뾶��Point��Spot��������ǿ��˥��Ϊ0ʱ�ľ���

		float m_SpotOuterAngle = 20.0f;				//Spot���Žǣ���Ӱ���Ե��[1, 179]
		float m_SpotInnerAngle = 18.3f;				//Spot���Žǣ���Ӱ�ڱ�Ե��= Outer - Outer / 12

		float m_Intensity = 1.0f;					//����ǿ��
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