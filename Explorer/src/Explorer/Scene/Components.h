#pragma once

#include <glm/glm.hpp>

#include <glm/gtc/matrix_transform.hpp>

namespace Explorer
{
	/// <summary>
	/// 组件
	/// </summary>
	class Component
	{
	private:
		std::string m_Name;
	public:
		Component(const std::string& name = "Component") :m_Name(name) {}
		virtual ~Component() = default;

		inline const std::string& GetName() const { return m_Name; }
		inline void SetName(const std::string& name) { m_Name = name; }
	};

	/// <summary>
	/// 转换组件
	/// </summary>
	class Transform :public Component
	{
	public:
		glm::vec3 m_Position{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation{ 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale{ 1.0f, 1.0f, 1.0f };

		glm::mat4 m_Transform{ 1.0f };

		Transform() :Component("Transform") 
		{
			CalculateTransform();
		}

		Transform(const glm::vec3& position, const glm::vec3& rotation, const glm::vec3& scale) 
			:Component("Transform"), m_Position(position), m_Rotation(rotation), m_Scale(scale)
		{
			CalculateTransform();
		}

		Transform(const Transform&) :Component("Transform") {}
		Transform(const glm::mat4& transform) :Component("Transform"), m_Transform(transform) {}

		void CalculateTransform()
		{
			m_Transform = glm::translate(glm::mat4(1.0f), m_Position)
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.x), glm::vec3(1.0f, 0, 0))
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.y), glm::vec3(0, 1.0f, 0))
				* glm::rotate(glm::mat4(1.0f), glm::radians(m_Rotation.z), glm::vec3(0, 0, 1.0f));
		}

		operator glm::mat4& () { return m_Transform; }
		operator const glm::mat4& () const { return m_Transform; }
	};

	/// <summary>
	/// 图片渲染器组件
	/// </summary>
	class SpriteRenderer :public Component
	{
	public:
		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };

		SpriteRenderer() :Component("Sprite Renderer") {}
		SpriteRenderer(const SpriteRenderer&) :Component("Sprite Renderer") {}
		SpriteRenderer(const glm::vec4& color) :Component("Sprite Renderer"), m_Color(color) {}
	};
}