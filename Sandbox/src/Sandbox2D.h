#pragma once

#include "Explorer.h"

/// <summary>
/// 2D渲染测试层
/// </summary>
class Sandbox2D :public Explorer::Layer
{
private:
	Explorer::CameraController m_CameraController;		//相机控制器

	//Temp
	std::shared_ptr<Explorer::VertexArray> m_SquareVA;			//正方形顶点数组
	std::shared_ptr<Explorer::Shader> m_FlatColorShader;		//正方形着色器

	glm::vec3 m_SquarePosition = { 0.0f, 0.0f, 0.0f};
	glm::vec3 m_SquareRotation = { 0.0f, 0.0f, 0.0f };
	glm::vec3 m_SquareScale = { 1.0f, 1.0f, 1.0f };

	glm::vec4 m_SquareColor = { 0.2f, 0.3f, 0.8f, 1.0f };
public:
	Sandbox2D();
	virtual ~Sandbox2D() = default;

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