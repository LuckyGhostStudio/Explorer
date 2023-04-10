#pragma once

#include "Explorer/Renderer/Texture.h"

namespace Explorer
{
	/// <summary>
	/// �����������
	/// </summary>
	class Component
	{
	protected:
		std::string m_Name;					//�����
		std::shared_ptr<Texture2D> m_Icon;	//���ͼ��
		bool m_Enable = true;				//�Ƿ��������
		bool m_SelectableEnable = true;		//�Ƿ��ѡ������������Ϊfalse ���������ѡ���Ƿ����ã�û��ѡ�����ù�ѡ���������ã�
		
		Component() = default;
		
		Component(const Component&) = default;
		Component(const std::string& name) :m_Name(name) {}

		virtual ~Component() = default;
		
		virtual void SetName() = 0;

		/// <summary>
		/// ����IconͼƬ
		/// </summary>
		virtual void SetIcon() = 0;
	public:
		virtual const std::string& GetName() = 0;
		virtual std::shared_ptr<Texture2D>& GetIcon() = 0;

		bool GetEnable() const { return m_Enable; }
		bool& GetEnable_Ref() { return m_Enable; }
		void SetEnable(bool enable) { m_Enable = enable; }

		virtual bool GetSelectableEnable() { return m_SelectableEnable; }
	};
}