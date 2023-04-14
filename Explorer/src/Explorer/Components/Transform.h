#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#define GLM_ENABLE_EXPERIMENTAL
#include <glm/gtx/quaternion.hpp>

#include "Explorer/Components/Component.h"

namespace Explorer
{
	/// <summary>
	/// ת�����
	/// </summary>
	class Transform :public Component
	{
	private:
		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Rotation = { 0.0f, 0.0f, 0.0f };
		glm::vec3 m_Scale = { 1.0f, 1.0f, 1.0f };
	private:
		virtual void SetName() override { m_Name = "Transform"; }

		/// <summary>
		/// ����Transform���ͼ��
		/// </summary>
		virtual void SetIcon() override { m_Icon = std::make_shared<Texture2D>("Resources/Icons/Components/Transform_Icon.png"); }
	public:
		Transform() = default;
		Transform(const Transform&) = default;
		Transform(const glm::vec3& position) :m_Position(position) {}
		Transform(const glm::vec3& position, const glm::vec3& rotation) :m_Position(position), m_Rotation(rotation) {}

		virtual const std::string& GetName() override { SetName(); return m_Name; }
		virtual std::shared_ptr<Texture2D>& GetIcon() override { SetIcon(); return m_Icon; }

		virtual bool GetSelectableEnable() override 
		{
			m_SelectableEnable = false;		//���ù�ѡ��
			return m_SelectableEnable; 
		}

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