#include <Explorer.h>

#include "imgui/imgui.h"

/// <summary>
/// 示例层
/// </summary>
class ExampleLayer :public Explorer::Layer
{
public:
	ExampleLayer() :Layer("Example") 
	{ 
		
	}

	void OnUpdate() override
	{
		if (Explorer::Input::IsKeyPressed(EXP_KEY_F)) {
			EXP_TRACE("{0} key is pressed!(poll)", EXP_KEY_F);
		}
	}

	void OnEvent(Explorer::Event& event) override
	{
		if (event.GetEventType() == Explorer::EventType::KeyPressed) {	//按键按下事件
			Explorer::KeyPressedEvent& e = (Explorer::KeyPressedEvent&)event;
			if (e.GetKeyCode() == EXP_KEY_F) {
				EXP_TRACE("{0} key is pressed!(event)", EXP_KEY_F);
			}
			EXP_TRACE("{0}", (char)e.GetKeyCode());
		}
	}
};

class Sandbox :public Explorer::Application
{
public:
	Sandbox()
	{
		PushLayer(new ExampleLayer());	//添加到层栈
	}

	~Sandbox()
	{

	}
};

Explorer::Application* Explorer::CreateApplication()
{
	return new Sandbox();
}