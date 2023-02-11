#include <Explorer.h>

/// <summary>
/// Ê¾Àý²ã
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
		PushLayer(new ExampleLayer());	//Ìí¼Óµ½²ãÕ»
		PushLayer(new Explorer::ImGuiLayer());	//Ìí¼ÓImGui²ãÎª¸²¸Ç²ã
	}

	~Sandbox()
	{

	}
};

Explorer::Application* Explorer::CreateApplication()
{
	return new Sandbox();
}