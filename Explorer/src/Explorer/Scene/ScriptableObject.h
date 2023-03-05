#pragma once

#include "Object.h"

namespace Explorer
{
	/// <summary>
	/// �ɽű����������б��ؽű��ĸ���
	/// </summary>
	class ScriptableObject
	{
	private:
		friend class Scene;
		Object m_Object;	//�ű����صĶ���
	public:
		virtual ~ScriptableObject() {}

		/// <summary>
		/// ����T�������
		/// </summary>
		/// <typeparam name="T">�������</typeparam>
		/// <returns>���</returns>
		template<typename T>
		T& GetComponent()
		{
			return m_Object.GetComponent<T>();
		}
	protected:
		/// <summary>
		/// ʵ�崴��ʱ����
		/// </summary>
		virtual void OnCreate() {}

		/// <summary>
		/// ʵ�崴����ÿ֡����
		/// </summary>
		/// <param name="dt">֡���</param>
		virtual void OnUpdate(DeltaTime dt) {}

		/// <summary>
		/// ʵ������ʱ����
		/// </summary>
		virtual void OnDestroy() {}
	};
}