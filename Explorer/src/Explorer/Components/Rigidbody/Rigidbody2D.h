#pragma once

#include "Explorer/Components/Component.h"

namespace Explorer
{
	/// <summary>
	/// 2D����
	/// </summary>
	class Rigidbody2D :public Component
	{
	public:
		/// <summary>
		/// ��������
		/// </summary>
		enum class BodyType
		{
			Dynamic = 0,	//��̬
			Static = 1,		//��̬����ֹ��
			Kinematic = 2	//����ѧ�ģ���������Ӱ��
		};
	private:
		BodyType m_Type = BodyType::Dynamic;	//��������
		bool m_FreezeRotation = false;			//������ת z��
		void* m_RuntimeBody = nullptr;			//����ʱ���壺ָ������ʱ���Լ�
	private:
		virtual void SetName() override { m_Name = "Rigidbody 2D"; }

		/// <summary>
		/// ����Transform���ͼ��
		/// </summary>
		virtual void SetIcon() override { m_Icon = std::make_shared<Texture2D>("Resources/Icons/Components/Rigidbody_Icon.png"); }
	public:
		Rigidbody2D() = default;
		Rigidbody2D(const Rigidbody2D&) = default;

		virtual const std::string& GetName() override { SetName(); return m_Name; }
		virtual std::shared_ptr<Texture2D>& GetIcon() override { SetIcon(); return m_Icon; }

		virtual bool GetSelectableEnable() override
		{
			m_SelectableEnable = false;		//���ù�ѡ��
			return m_SelectableEnable;
		}

		BodyType GetBodyType() const { return m_Type; }
		void SetBodyType(BodyType type) { m_Type = type; }

		bool GetFreezeRotation() const { return m_FreezeRotation; }
		bool& GetFreezeRotation_Ref() { return m_FreezeRotation; }
		void SetFreezeRotation(bool freeze) { m_FreezeRotation = freeze; }

		void* GetRuntimeBody() { return m_RuntimeBody; }
		void SetRuntimeBody(void* runtimeBody) { m_RuntimeBody = runtimeBody; }
	};
}