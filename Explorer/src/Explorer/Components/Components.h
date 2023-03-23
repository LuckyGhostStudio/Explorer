#pragma once

#include "Explorer/Scene/ScriptableObject.h"

namespace Explorer
{
	/// <summary>
	/// 名字组件
	/// </summary>
	class Name
	{
	public:
		std::string m_Name;

		Name() = default;
		Name(const Name&) = default;
		Name(const std::string& name) :m_Name(name) {}
	};

	/// <summary>
	/// 图片渲染器组件
	/// </summary>
	class SpriteRenderer
	{
	public:
		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRenderer() = default;
		SpriteRenderer(const SpriteRenderer&) = default;
		SpriteRenderer(const glm::vec4& color) :m_Color(color) {}
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