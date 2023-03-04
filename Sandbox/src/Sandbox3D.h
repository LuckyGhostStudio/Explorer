#pragma once

#include "Explorer.h"

/// <summary>
/// 2D��Ⱦ���Բ�
/// </summary>
class Sandbox3D :public Explorer::Layer
{
private:
	//Explorer::CameraController m_CameraController;	//���������

	std::shared_ptr<Explorer::Scene> m_ActiveScene;
	Explorer::Object m_CameraObject;					//�������

	glm::vec3 m_CubePosition = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_CubeRotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_CubeScale = { 1.0f, 1.0f, 1.0f };

	glm::vec4 m_CubeColor = { 1.0f, 1.0f, 1.0f, 1.0f };

public:
	Sandbox3D();
	virtual ~Sandbox3D() = default;

	/// <summary>
	/// ���ò���ӵ���ջʱ����
	/// </summary>
	virtual void OnAttach() override;

	/// <summary>
	/// ���ò�Ӳ�ջ�Ƴ�ʱ����
	/// </summary>
	virtual void OnDetach() override;

	/// <summary>
	/// �ò����ʱ���ã�ÿ֡����
	/// </summary>
	/// <param name="dt">֡���</param>
	virtual void OnUpdate(Explorer::DeltaTime dt) override;

	/// <summary>
	/// ��ȾImGui
	/// </summary>
	virtual void OnImGuiRender() override;

	/// <summary>
	/// �¼������������¼�
	/// </summary>
	/// <param name="event">�¼�</param>
	virtual void OnEvent(Explorer::Event& event) override;
};