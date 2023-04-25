#pragma once

#include "Explorer/Components/Component.h"
#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// Circle2D��ײ��
	/// </summary>
	class CircleCollider2D :public Component
	{
	private:
		Circle m_Circle;		//��ײ��Բ
		bool m_RenderCircle;	//����Circle

		glm::vec2 m_Offset = { 0.0f, 0.0f };	//ƫ�������������������
		float m_Radius = 0.5f;					//�뾶

		//TODO:�������Դ������������
		float m_Density = 1.0f;					//�ܶ�	kg/m^2
		float m_Friction = 0.5f;				//Ħ���� [0, 1]
		float m_Restitution = 0.0f;				//�ָ�ϵ����Ϊ0��û�е��� [0, 1]
		float m_RestitutionThreshold = 0.5f;	//�ָ���ֵ���ٶȵ��ڴ�ֵ��ֹͣ��������

		void* m_RuntimeFixture = nullptr;		//����ʱ��ײ�壺ָ���Լ�
	private:
		virtual void SetName() override { m_Name = "Circle Collider 2D"; }

		/// <summary>
		/// ����Transform���ͼ��
		/// </summary>
		virtual void SetIcon() override { m_Icon = std::make_shared<Texture2D>("Resources/Icons/Components/CircleCollider2D_Icon.png"); }
	public:
		CircleCollider2D() = default;
		CircleCollider2D(const CircleCollider2D&) = default;

		virtual const std::string& GetName() override { SetName(); return m_Name; }
		virtual std::shared_ptr<Texture2D>& GetIcon() override { SetIcon(); return m_Icon; }

		Circle& GetCircle() { return m_Circle; }
		void SetCircle(const Circle& circle) { m_Circle = circle; }

		bool GetRenderCircle() const { return m_RenderCircle; }
		bool& GetRenderCircle_Ref() { return m_RenderCircle; }
		void SetRenderCircle(float render) { m_RenderCircle = render; }

		glm::vec2& GetOffset() { return m_Offset; }
		void SetOffset(const glm::vec2& offset) { m_Offset = offset; }

		float GetRadius() const { return m_Radius; }
		float& GetRadius_Ref() { return m_Radius; }
		void SetRadius(float radius) { m_Radius = radius; }

		float GetDensity() const { return m_Density; }
		float& GetDensity_Ref() { return m_Density; }
		void SetDensity(float density) { m_Density = density; }

		float GetFriction() const { return m_Friction; }
		float& GetFriction_Ref() { return m_Friction; }
		void SetFriction(float friction) { m_Friction = friction; }

		float GetRestitution() const { return m_Restitution; }
		float& GetRestitution_Ref() { return m_Restitution; }
		void SetRestitution(float restitution) { m_Restitution = restitution; }

		float GetRestitutionThreshold() const { return m_RestitutionThreshold; }
		float& GetRestitutionThreshold_Ref() { return m_RestitutionThreshold; }
		void SetRestitutionThreshold(float threshold) { m_RestitutionThreshold = threshold; }

		void* GetRuntimeFixture() { return m_RuntimeFixture; }
		void SetRuntimeFixture(void* runtimeFixture) { m_RuntimeFixture = runtimeFixture; }
	};
}