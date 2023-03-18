#pragma once

#include "Explorer/Core/DeltaTime.h"
#include "Explorer/Events/Event.h"
#include "Explorer/Events/MouseEvent.h"

#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// �༭�����
	/// </summary>
	class EditorCamera
	{
	private:
		float m_FOV = 45.0f;					//�����ֱ�Ž�
		float m_Near = 0.01f;					//���ü�ƽ��
		float m_Far = 1000.0f;					//Զ�ü�ƽ��
		float m_AspectRatio = 1280.0f / 720.0f;	//��Ļ���߱�

		glm::mat4 m_ViewMatrix;								//��ͼ����
		glm::mat4 m_ProjectionMatrix = glm::mat4(1.0f);		//ͶӰ����

		glm::vec3 m_Position = { 0.0f, 0.0f, 0.0f };		//���λ��
		glm::vec3 m_FocalPoint = { 0.0f, 0.0f, 0.0f };		//����λ��

		glm::vec2 m_InitialMousePosition = { 0.0f, 0.0f };	//����ʼλ��

		float m_Distance = 5.0f;					//����뽹�����
		float m_Pitch = 0.0f, m_Yaw = 0.0f;

		float m_ViewportWidth = 1280.0f;	//�ӿڿ�
		float m_ViewportHeight = 720;		//�ӿڸ�
	private:
		/// <summary>
		/// ����ͶӰ����
		/// </summary>
		void UpdateProjection();

		/// <summary>
		/// ������ͼ����
		/// </summary>
		void UpdateView();

		/// <summary>
		/// �����ֹ���ʱ����
		/// </summary>
		/// <param name="e">�������¼�</param>
		/// <returns>�������</returns>
		bool OnMouseScroll(MouseScrolledEvent& e);

		void ViewPan(const glm::vec2& delta);
		void ViewRotate(const glm::vec2& delta);
		void ViewZoom(float delta);

		/// <summary>
		/// �������λ��
		/// </summary>
		/// <returns></returns>
		glm::vec3 CalculatePosition() const;

		std::pair<float, float> PanSpeed() const;
		float RotationSpeed() const;
		float ZoomSpeed() const;
	public:
		EditorCamera() = default;

		/// <summary>
		/// �༭�����
		/// </summary>
		/// <param name="fov">��ֱ�Ž�</param>
		/// <param name="aspectRatio">��Ļ���߱�</param>
		/// <param name="nearClip">���ü�ƽ��</param>
		/// <param name="farClip">Զ�ü�ƽ��</param>
		EditorCamera(float fov, float aspectRatio, float nearClip, float farClip);

		/// <summary>
		/// ���������ÿ֡
		/// </summary>
		/// <param name="dt">֡���</param>
		void OnUpdate(DeltaTime dt);

		/// <summary>
		/// �¼��ص�����
		/// </summary>
		/// <param name="e">�¼�</param>
		void OnEvent(Event& e);

		inline void SetViewportSize(float width, float height) { m_ViewportWidth = width; m_ViewportHeight = height; UpdateProjection(); }
		
		inline float GetDistance() const { return m_Distance; }
		inline void SetDistance(float distance) { m_Distance = distance; }

		const glm::mat4& GetViewMatrix() const { return m_ViewMatrix; }
		const glm::mat4& GetProjection() const { return m_ProjectionMatrix; }
		glm::mat4 GetViewProjectionMatrix() const { return m_ProjectionMatrix * m_ViewMatrix; }

		glm::vec3 GetUpDirection() const;
		glm::vec3 GetRightDirection() const;
		glm::vec3 GetForwardDirection() const;
		const glm::vec3& GetPosition() const { return m_Position; }
		glm::quat GetOrientation() const;

		float GetPitch() const { return m_Pitch; }
		float GetYaw() const { return m_Yaw; }
	};
}