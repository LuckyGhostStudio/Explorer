#pragma once

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

#include "Explorer/Scene/ScriptableObject.h"

namespace Explorer
{
	/// <summary>
	/// ת�����
	/// </summary>
	class Transform
	{
	public:
		std::string m_Name = "Transform";

		glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale{ 1.0f, 1.0f, 1.0f };

		glm::mat4 m_Transform{ 1.0f };

		Transform()
		{
			CalculateTransform();
		}

		Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) 
			:m_Position(position), m_Rotation(rotation), m_Scale(scale)
		{
			CalculateTransform();
		}

		Transform(const Transform&) {}
		Transform(const glm::mat4& transform) :m_Transform(transform) {}

		/// <summary>
		/// ����Transform����
		/// </summary>
		void CalculateTransform()
		{
			m_Transform = glm::translate(glm::mat4(1.0f), m_Position)
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1.0f, 0, 0))
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0, 1.0f, 0))
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0, 0, 1.0f))
				* glm::scale(glm::mat4(1.0f), m_Scale);
		}

		operator glm::mat4& () { return m_Transform; }
		operator const glm::mat4& () const { return m_Transform; }
	};

	/// <summary>
	/// ͼƬ��Ⱦ�����
	/// </summary>
	class SpriteRenderer
	{
	public:
		std::string m_Name = "Sprite Renderer";

		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRenderer() {}
		SpriteRenderer(const SpriteRenderer&) {}
		SpriteRenderer(const glm::vec4& color) :m_Color(color) {}
	};

	class NativeScript
	{
	public:
		std::string m_Name = "NativeScript";

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