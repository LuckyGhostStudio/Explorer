#pragma once

#include "Explorer.h"

/// <summary>
/// 2D渲染测试层
/// </summary>
class Sandbox3D :public Explorer::Layer
{
private:
	//Explorer::CameraController m_CameraController;	//相机控制器

	std::shared_ptr<Explorer::Scene> m_ActiveScene;
	Explorer::Object m_CameraObject;					//相机对象

	glm::vec3 m_CubePosition = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_CubeRotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_CubeScale = { 1.0f, 1.0f, 1.0f };

	glm::vec4 m_CubeColor = { 1.0f, 1.0f, 1.0f, 1.0f };

public:
	Sandbox3D();
	virtual ~Sandbox3D() = default;

	/// <summary>
	/// 将该层添加到层栈时调用
	/// </summary>
	virtual void OnAttach() override;

	/// <summary>
	/// 将该层从层栈移除时调用
	/// </summary>
	virtual void OnDetach() override;

	/// <summary>
	/// 该层更新时调用：每帧调用
	/// </summary>
	/// <param name="dt">帧间隔</param>
	virtual void OnUpdate(Explorer::DeltaTime dt) override;

	/// <summary>
	/// 渲染ImGui
	/// </summary>
	virtual void OnImGuiRender() override;

	/// <summary>
	/// 事件函数：接收事件
	/// </summary>
	/// <param name="event">事件</param>
	virtual void OnEvent(Explorer::Event& event) override;
};