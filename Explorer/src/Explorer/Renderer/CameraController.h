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
		float m_AspectRatio;			//���߱ȣ�X/Y��
		float m_ZoomLevel = 1.0f;		//���ű�����Y��
		Camera m_Camera;				//���

		bool m_Rotation;				//�Ƿ����ת

		glm::vec3 m_CameraPosition = { 0.0f, 0.0f, 0.0f };	//���λ��
		float m_CameraRotation = 0.0f;						//�����ת��z�ᣩ
		float m_CameraTranslationSpeed = 1.5f;				//����ƶ��ٶ�
		float m_CameraRotationSpeed = 90.0f;				//�����ת�ٶ�
	public:
		/// <summary>
		/// �������������
		/// </summary>
		/// <param name="aspectRatio">���߱ȣ�X/Y��</param>
		/// <param name="rotation">�Ƿ����ת��false��</param>
		CameraController(float aspectRatio, bool rotation = false);

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

		Camera& GetCamera() { return m_Camera; }
		const Camera& GetCamera() const { return m_Camera; }

		float GetZoomLevel() const { return m_ZoomLevel; }
		void SetZoomLevel(float level) { m_ZoomLevel = level; }
	private:
		/// <summary>
		/// �����ֹ���
		/// </summary>
		/// <param name="e">�����ֹ����¼�</param>
		/// <returns>�¼��������</returns>
		bool OnMouseScrolled(MouseScrolledEvent& e);

		/// <summary>
		/// ��������
		/// </summary>
		/// <param name="e">���������¼�</param>
		/// <returns>�¼��������</returns>
		bool OnWindowResized(WindowResizeEvent& e);
	};
}