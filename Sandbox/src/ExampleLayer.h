#pragma once

#include "Explorer.h"

/// <summary>
/// ʾ����
/// </summary>
class ExampleLayer :public Explorer::Layer
{
private:
	Explorer::ShaderLibrary m_ShaderLibrary;				//��ɫ����
	std::shared_ptr<Explorer::Shader> m_Shader;				//��������ɫ��
	std::shared_ptr<Explorer::VertexArray> m_VertexArray;	//�����ζ�������
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