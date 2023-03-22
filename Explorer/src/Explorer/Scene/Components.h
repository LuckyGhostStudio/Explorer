#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

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
	/// 转换组件
	/// </summary>
	class Transform
	{
	public:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };

		Transform() = default;
		Transform(const Transform&) = default;
		Transform(const glm::vec3& position) :m_Position(position) {}
		Transform(const glm::vec3& position, const glm::vec3& rotation) :m_Position(position), m_Rotation(rotation) {}

		/// <summary>
		/// 返回Transform矩阵
		/// </summary>
		/// <returns>Transform矩阵</returns>
		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(m_Rotation)));	//旋转矩阵：四元数旋转

			return glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);
		}

		/// <summary>
		/// 返回right方向向量（x+）
		/// </summary>
		/// <returns>right方向向量</returns>
		glm::vec3 GetRightDirection() const
		{
			return glm::rotate(glm::quat(glm::radians(m_Rotation)), glm::vec3(1.0f, 0.0f, 0.0f));
		}

		/// <summary>
		/// 返回up方向向量（y+）
		/// </summary>
		/// <returns>up方向向量</returns>
		glm::vec3 GetUpDirection() const
		{
			return glm::rotate(glm::quat(glm::radians(m_Rotation)), glm::vec3(0.0f, 1.0f, 0.0f));
		}

		/// <summary>
		/// 返回forward方向向量（z+）
		/// </summary>
		/// <returns>forward方向向量</returns>
		glm::vec3 GetForwardDirection() const
		{
			return glm::rotate(glm::quat(glm::radians(m_Rotation)), glm::vec3(0.0f, 0.0f, 1.0f));
		}
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