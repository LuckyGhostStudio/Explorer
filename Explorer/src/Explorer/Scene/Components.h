#pragma once

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include "Explorer/Scene/ScriptableObject.h"

namespace Explorer
{
	/// <summary>
	/// �������
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
	/// ת�����
	/// </summary>
	class Transform
	{
	public:
		glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale{ 1.0f, 1.0f, 1.0f };

		Transform() = default;
		Transform(const Transform&) {}
		Transform(const glm::vec3& position) :m_Position(position) {}

		/// <summary>
		/// ����Transform����
		/// </summary>
		/// <returns>Transform����</returns>
		glm::mat4& GetTransform() const
		{
			glm::mat4 rotation = glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), { 1, 0, 0 })
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), { 0, 1, 0 })
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), { 0, 0, 1 });

			return glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);
		}
	};

	/// <summary>
	/// ͼƬ��Ⱦ�����
	/// </summary>
	class SpriteRenderer
	{
	public:
		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRenderer() {}
		SpriteRenderer(const SpriteRenderer&) {}
		SpriteRenderer(const glm::vec4& color) :m_Color(color) {}
	};

	class NativeScript
	{
	public:
		ScriptableObject* Instance = nullptr;		//�ɽű���ʵ��ʵ�������ڵ��ýű��ڵĺ���

		ScriptableObject* (*InstantiateScript)();	//ʵ�����ű�����������ʵ����ScriptableEntity�ű�
		void (*DestroyScript)(NativeScript*);		//���ٽű���������������ScriptableEntity�ű���ʵ��

		/// <summary>
		/// ��T���͵Ľű����ű����
		/// </summary>
		/// <typeparam name="T">�ű�����</typeparam>
		template<typename T>
		void Bind()
		{
			InstantiateScript = []() { return static_cast<ScriptableObject*>(new T()); };	//ʵ�����ű���
			DestroyScript = [](NativeScript* script) { delete script->Instance; script->Instance = nullptr; };	//���ٽű����ʵ��
		}
	};
}