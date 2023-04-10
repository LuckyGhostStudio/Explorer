#pragma once

#include <glm/glm.hpp>

#include "Explorer/Components/Component.h"

namespace Explorer
{
	/// <summary>
	/// ������
	/// </summary>
	class Camera :public Component
	{
	public:
		/// <summary>
		/// ������־
		/// </summary>
		enum class ClearFlag { Color = 0, Skybox = 1 };

		/// <summary>
		/// ͶӰ����
		/// </summary>
		enum class ProjectionType { Perspective = 0, Orthographic = 1 };
	private:
		bool m_Primary = true;	//�������

		ClearFlag m_ClearFlag = ClearFlag::Color;						//��������
		glm::vec4 m_BackgroundColor = { 0.2f, 0.3f, 0.5f, 1.0f };		//������ɫ��������ɫ
		ProjectionType m_ProjectionType = ProjectionType::Perspective;	//ͶӰ����

		float m_Fov = glm::radians(45.0f);	//͸������Žǣ���ֱ���򣨻��ȣ�
		float m_Size = 5.0f;				//��������ߴ磺��ֱ����һ�볤��
		
		float m_Near = 0.01f;				//���ü�ƽ��
		float m_Far = 1000.0f;				//Զ�ü�ƽ��

		float m_AspectRatio = 0.0f;			//��Ļ��߱ȣ�X/Y��

		glm::mat4 m_Projection;				//ͶӰ����
	private:
		/// <summary>
		/// ���¼���ͶӰ����
		/// </summary>
		void RecalculateProjection();

		virtual void SetName() override { m_Name = "Camera"; }

		/// <summary>
		/// ����Camera���ͼ��
		/// </summary>
		virtual void SetIcon() override { m_Icon = std::make_shared<Texture2D>("asserts/textures/defaults/Icons/Components/Camera_Icon.png"); }
	public:
		Camera();

		virtual const std::string& GetName() override { SetName(); return m_Name; }
		virtual std::shared_ptr<Texture2D>& GetIcon() override { SetIcon(); return m_Icon; }

		/// <summary>
		/// ��������ͶӰ����->����ͶӰ����
		/// </summary>
		/// <param name="fov">��ֱ�ŽǴ�С</param>
		/// <param name="nearClip">���ü�ƽ��</param>
		/// <param name="farClip">Զ�ü�ƽ��</param>
		void SetPerspective(float fov, float nearClip, float farClip);

		/// <summary>
		/// ��������ͶӰ����->����ͶӰ����
		/// </summary>
		/// <param name="size">�ߴ磺��ֱ����</param>
		/// <param name="nearClip">���ü�ƽ��</param>
		/// <param name="farClip">Զ�ü�ƽ��</param>
		void SetOrthographic(float size, float nearClip, float farClip);

		/// <summary>
		/// �����ӿڴ�С->����ͶӰ����
		/// </summary>
		/// <param name="width">��</param>
		/// <param name="height">��</param>
		void SetViewportSize(uint32_t width, uint32_t height);

		const glm::mat4& GetProjection() const { return m_Projection; }
		
		bool IsPrimary() const { return m_Primary; }
		bool& IsPrimary_Ref() { return m_Primary; }
		void SetPrimary(bool primary) { m_Primary = primary; }

		ClearFlag GetClearFlag() const { return m_ClearFlag; }
		void SetClearFlag(const ClearFlag flag) { m_ClearFlag = flag; }

		glm::vec4& GetBackgroundColor() { return m_BackgroundColor; }
		void SetBackgroundColor(const glm::vec4& color) { m_BackgroundColor = color; }

		ProjectionType GetProjectionType() const { return m_ProjectionType; }
		void SetProjectionType(ProjectionType type) { m_ProjectionType = type; RecalculateProjection(); }

		float GetNearClip() const { return m_Near; }
		float& GetNearClip_Ref() { return m_Near; }
		void SetNearClip(float nearClip) { m_Near = nearClip; RecalculateProjection(); }

		float GetFarClip() const { return m_Far; }
		float& GetFarClip_Ref() { return m_Far; }
		void SetFarClip(float farClip) { m_Far = farClip; RecalculateProjection(); }

		float GetSize() const { return m_Size; }
		float& GetSize_Ref() { return m_Size; }
		void SetSize(float size) { m_Size = size; RecalculateProjection(); }

		float GetFOV() const { return m_Fov; }
		float& GetFOV_Ref() { return m_Fov; }
		void SetFOV(float fov) { m_Fov = fov; RecalculateProjection(); }
	};
}