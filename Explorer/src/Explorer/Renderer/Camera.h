#pragma once

#include <glm/glm.hpp>

#include "Explorer/Scene/Components.h"

namespace Explorer
{
	/// <summary>
	/// ������־
	/// </summary>
	enum class ClearFlag
	{
		Color = 0,	//��ɫ
		Skybox = 1	//��պ�
	};

	/// <summary>
	/// �������
	/// </summary>
	enum class CameraType
	{
		Orthographic = 0,	//����
		Perspective = 1		//͸��
	};

	/// <summary>
	/// ���
	/// </summary>
	class Camera
	{
	public:
		//Temp
		bool m_Primary = true;	//�������
	private:
		ClearFlag m_ClearFlag;			//��������
		glm::vec4 m_BackgroundColor;	//������ɫ��������ɫ
		CameraType m_Type;				//�������
		glm::mat4 m_Projection;			//ͶӰ����
		float m_AspectRatio = 0.0f;		//��Ļ��߱ȣ�X/Y��
		float m_Near = -1.0f;			//���ü�ƽ��
		float m_Far = 1.0f;				//Զ�ü�ƽ��

		float m_Size = 5.0f;			//��������ߴ磺��ֱ����һ�볤��
		float m_Fov = 45.0f;			//͸�������ֱ�����Ž�
	private:
		/// <summary>
		/// ���¼���ͶӰ����
		/// </summary>
		void RecalculateProjection();
	public:
		Camera();

		/// <summary>
		/// ��������ͶӰ����->����ͶӰ����
		/// </summary>
		/// <param name="size">�ߴ磺��ֱ����</param>
		/// <param name="nearClip">���ü�ƽ��</param>
		/// <param name="farClip">Զ�ü�ƽ��</param>
		void SetOrthographic(float size, float nearClip, float farClip);

		/// <summary>
		/// ��������ͶӰ����->����ͶӰ����
		/// </summary>
		/// <param name="fov">��ֱ�ŽǴ�С</param>
		/// <param name="aspectRatio">��߱�</param>
		/// <param name="nearClip">���ü�ƽ��</param>
		/// <param name="farClip">Զ�ü�ƽ��</param>
		void SetPerspective(float fov, float aspectRatio, float nearClip, float farClip);

		/// <summary>
		/// �����ӿڴ�С->����ͶӰ����
		/// </summary>
		/// <param name="width">��</param>
		/// <param name="height">��</param>
		void SetViewportSize(uint32_t width, uint32_t height);

		ClearFlag GetClearFlag() const { return m_ClearFlag; }
		void SetClearFlag(const ClearFlag& flag) { m_ClearFlag = flag; }

		const glm::vec4& GetBackgroundColor() const { return m_BackgroundColor; }
		void SetBackgroundColor(const glm::vec4& background) { m_BackgroundColor = background; }

		CameraType GetType() const { return m_Type; }
		void SetType(const CameraType& type) { m_Type = type; RecalculateProjection(); }

		const glm::mat4& GetProjection() const { return m_Projection; }

		float GetNearClip() const { return m_Near; }
		void SetNearClip(float nearClip) { m_Near = nearClip; RecalculateProjection(); }

		float GetFarClip() const { return m_Far; }
		void SetFarClip(float farClip) { m_Far = farClip; RecalculateProjection(); }

		float GetSize() const { return m_Size; }
		void SetSize(float size) { m_Size = size; RecalculateProjection(); }

		float GetFOV() const { return m_Fov; }
		void SetFOV(float fov) { m_Fov = fov; RecalculateProjection(); }
	};
}