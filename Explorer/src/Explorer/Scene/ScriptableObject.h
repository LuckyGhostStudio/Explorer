#pragma once

#include "Object.h"

namespace Explorer
{
	/// <summary>
	/// 可脚本化对象：所有本地脚本的父类
	/// </summary>
	class ScriptableObject
	{
	private:
		friend class Scene;
		Object m_Object;	//脚本挂载的对象
	public:
		virtual ~ScriptableObject() {}

		/// <summary>
		/// 返回T类型组件
		/// </summary>
		/// <typeparam name="T">组件类型</typeparam>
		/// <returns>组件</returns>
		template<typename T>
		T& GetComponent()
		{
			return m_Object.GetComponent<T>();
		}
	protected:
		/// <summary>
		/// 实体创建时调用
		/// </summary>
		virtual void OnCreate() {}

		/// <summary>
		/// 实体创建后每帧调用
		/// </summary>
		/// <param name="dt">帧间隔</param>
		virtual void OnUpdate(DeltaTime dt) {}

		/// <summary>
		/// 实体销毁时调用
		/// </summary>
		virtual void OnDestroy() {}
	};
}