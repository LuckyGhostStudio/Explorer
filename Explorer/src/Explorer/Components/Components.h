#pragma once

#include "Explorer/Scene/ScriptableObject.h"

namespace Explorer
{
	/// <summary>
	/// 自身属性组件：Object的固有属性
	/// </summary>
	class Self
	{
	private:
		std::string m_ObjectName;	//挂载此组件的物体名
		bool m_ObjectEnable;		//是否启用物体

		std::vector<entt::entity> m_ChildrenID{ entt::null };	//子物体ID TODO:添加父子物体
	public:
		Self() = default;
		Self(const Self&) = default;
		Self(const std::string& objectName, bool enableObject = true) 
			:m_ObjectName(objectName), m_ObjectEnable(enableObject) {}

		std::string& GetObjectName() { return m_ObjectName; }
		void SetObjectName(const std::string& name) { m_ObjectName = name; }

		bool GetObjectEnable() const { return m_ObjectEnable; }
		bool& GetObjectEnable_Ref() { return m_ObjectEnable; }
		void SetObjectEnable(bool enable) { m_ObjectEnable = enable; }
	};

	/// <summary>
	/// 本地脚本组件
	/// </summary>
	class NativeScript
	{
	public:
		ScriptableObject* Instance = nullptr;		//可脚本化实体实例：用于调用脚本内的函数

		ScriptableObject* (*InstantiateScript)();	//实例化脚本函数：用来实例化ScriptableEntity脚本
		void (*DestroyScript)(NativeScript*);		//销毁脚本函数：用来销毁ScriptableEntity脚本的实例

		/// <summary>
		/// 绑定T类型的脚本到脚本组件
		/// </summary>
		/// <typeparam name="T">脚本类型</typeparam>
		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableObject*>(new T()); };	//实例化脚本类
			DestroyScript = [](NativeScript* script) { delete script->Instance; script->Instance = nullptr; };	//销毁脚本类的实例
		}
	};
}