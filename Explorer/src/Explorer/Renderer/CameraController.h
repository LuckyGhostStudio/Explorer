#pragma once

#include "Explorer/Core/DeltaTime.h"
#include "Explorer/Renderer/Camera.h"

#include "Explorer/Events/ApplicationEvent.h"
#include "Explorer/Events/MouseEvent.h"

namespace Explorer
{
	/// <summary>
	/// ���������
	/// </summary>
	class CameraController
	{
	private:
		float m_AspectRatio;		//��߱ȣ�X/Y��
		float m_ZoomLevel = 1.0f;	//���ű�����Y��
		Camera m_Camera;			//���

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };	//���λ��
		glm::vec3 m_CameraRotation = { 0.0f, 0.0f, 0.0f };	//�����ת
		float m_CameraTranslationSpeed = 2.0f;				//����ƶ��ٶ�
		float m_CameraRotationSpeed = 90.0f;				//�����ת�ٶ�
	public:
		/// <summary>
		/// �������������
		/// </summary>
		/// <param name="aspectRatio">��߱ȣ�X/Y��</param>
		CameraController(float aspectRatio);

		/// <summary>
		/// ͸�����������
		/// </summary>
		/// <param name="fov">��ֱ�����Ž�</param>
		/// <param name="aspectRatio">��Ļ��߱�</param>
		/// <param name="clippingPlane">�ü�ƽ�棨����Զ��</param>
		CameraController(float fov, float aspectRatio, const glm::vec2& clippingPlane);

		/// <summary>
		/// ���£�ÿ֡����
		/// </summary>
		/// <param name="dt">֡���</param>
		void OnUpdate(DeltaTime dt);

		/// <summary>
		/// �¼��ص��������¼�����ʱ����
		/// </summary>
		/// <param name="e">�¼�</param>
		void OnEvent(Event& e);

		/// <summary>
		/// ���������С
		/// </summary>
		/// <param name="width">��</param>
		/// <param name="height">��</param>
		void OnResize(float width, float height);

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }

		/// <summary>
		/// �������ű���
		/// </summary>
		/// <param name="level">���ű���</param>
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		/// <summary>
		/// �����ֹ���
		/// </summary>
		/// <param name="e">�����ֹ����¼�</param>
		/// <returns>�¼�������</returns>
		bool OnMouseScrolled(MouseScrolledEvent& e);

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="e">���������¼�</param>
		/// <returns>�¼�������</returns>
		bool OnWindowResized(WindowResizeEvent& e);
	};
}