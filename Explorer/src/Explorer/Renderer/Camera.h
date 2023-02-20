#pragma once

#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// ���
	/// </summary>
	class Camera
	{
	private:
		glm::mat4 m_ProjectionMatrix;		//ͶӰ����
		glm::mat4 m_ViewMatrix;				//��ͼ��������任�������
		glm::mat4 m_ViewProjectionMatrix;	//��ͼͶӰ����p * v

		glm::vec3 m_Position = { 0.0f,0.0f,0.0f };	//λ��
		float m_Rotation = 0.0f;					//��ת��z�ᣩ

		/// <summary>
		/// ������ͼ����
		/// </summary>
		void RecalculateViewMatrix();
	public:
		/// <summary>
		/// �������
		/// </summary>
		/// <param name="left">��߽�</param>
		/// <param name="right">�ұ߽�</param>
		/// <param name="bottom">�±߽�</param>
		/// <param name="top">�ϱ߽�</param>
		Camera(float left, float right, float bottom, float top);

		const glm::vec3& GetPosition() const { return m_Position; }

		/// <summary>
		/// �������λ��
		/// </summary>
		/// <param name="position">λ��</param>
		void SetPosition(const glm::vec3& position)
		{
			m_Position = position;
			RecalculateViewMatrix();
		}

		float GetRotation() const { return m_Rotation; }

		/// <summary>
		/// ���������ת��z�ᣩ
		/// </summary>
		/// <param name="rotation">��ת�Ƕȣ�z�ᣩ</param>
		void SetRotation(float rotation)
		{
			m_Rotation = rotation;
			RecalculateViewMatrix();
		}

		const glm::mat4& GetProjectionMatrix() const { return m_ProjectionMatrix; }
		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetViewProjectionMatrix() const { return m_ViewProjectionMatrix; }
	};
}