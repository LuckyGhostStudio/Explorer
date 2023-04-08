#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

namespace Explorer
{
	/// <summary>
	/// ת�����
	/// </summary>
	class Transform
	{
	private:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };
	public:
		Transform() = default;
		Transform(const Transform&) = default;
		Transform(const glm::vec3& position) :m_Position(position) {}
		Transform(const glm::vec3& position, const glm::vec3& rotation) :m_Position(position), m_Rotation(rotation) {}

		glm::vec3& GetPosition() { return m_Position; }
		void SetPosition(const glm::vec3& position) { m_Position = position; }
		
		glm::vec3& GetRotation() { return m_Rotation; }
		void SetRotation(const glm::vec3& rotation) { m_Rotation = rotation; }

		glm::vec3& GetScale() { return m_Scale; }
		void SetScale(const glm::vec3& scale) { m_Scale = scale; }

		/// <summary>
		/// ����Transform����
		/// </summary>
		/// <returns>Transform����</returns>
		glm::mat4 GetTransform() const
		{
			glm::mat4 rotation = glm::toMat4(glm::quat(glm::radians(m_Rotation)));	//��ת������Ԫ����ת

			return glm::translate(glm::mat4(1.0f), m_Position) * rotation * glm::scale(glm::mat4(1.0f), m_Scale);
		}

		/// <summary>
		/// ����right����������x+��
		/// </summary>
		/// <returns>right��������</returns>
		glm::vec3 GetRightDirection() const
		{
			return glm::normalize(glm::rotate(glm::quat(glm::radians(m_Rotation)), glm::vec3(1.0f, 0.0f, 0.0f)));
		}

		/// <summary>
		/// ����up����������y+��
		/// </summary>
		/// <returns>up��������</returns>
		glm::vec3 GetUpDirection() const
		{
			return glm::normalize(glm::rotate(glm::quat(glm::radians(m_Rotation)), glm::vec3(0.0f, 1.0f, 0.0f)));
		}

		/// <summary>
		/// ����forward����������z+��
		/// </summary>
		/// <returns>forward��������</returns>
		glm::vec3 GetForwardDirection() const
		{
			return glm::normalize(glm::rotate(glm::quat(glm::radians(m_Rotation)), glm::vec3(0.0f, 0.0f, 1.0f)));
		}
	};
}