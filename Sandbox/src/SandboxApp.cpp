#include <Explorer.h>

/// <summary>
/// ʾ����
/// </summary>
class ExampleLayer :public Explorer::Layer
{
public:
	ExampleLayer() :Layer("Example") {}

	void OnUpdate() override
	{
		EXP_INFO("ExampleLayer::OnUpdate");
	}

	void OnEvent(Explorer::Event& event) override
	{
		EXP_TRACE("{0}", event);
	}
};

class Sandbox :public Explorer::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());	//��ӵ���ջ
		PushLayer(new Explorer::ImGuiLayer());	//���ImGui��Ϊ���ǲ�
	}

	~Sandbox()
	{

	}
};

Explorer::Application* Explorer::CreateApplication()
{
	return new Sandbox();
}