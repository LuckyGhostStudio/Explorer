#pragma once

#include "Explorer.h"

/// <summary>
/// 示例层
/// </summary>
class ExampleLayer :public Explorer::Layer
{
private:
	Explorer::ShaderLibrary m_ShaderLibrary;				//着色器库
	std::shared_ptr<Explorer::Shader> m_Shader;				//三角形着色器
	std::shared_ptr<Explorer::VertexArray> m_VertexArray;	//三角形顶点数组
	std::shared_ptr<Explorer::Shader> m_FlatColorShader;
	std::shared_ptr<Explorer::VertexArray> m_SquareVA;

	std::shared_ptr<Explorer::Texture2D> m_Texture, m_ChernoLogoTexture;

	glm::vec3 m_SquareColor = { 0.2f, 0.3f, 0.8f };
public:
	ExampleLayer();

	virtual void OnUpdate(Explorer::DeltaTime dt) override;

	virtual void OnImGuiRender() override;

	virtual void OnEvent(Explorer::Event& event) override;
};