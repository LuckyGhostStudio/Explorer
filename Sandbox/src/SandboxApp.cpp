#include <Explorer.h>
#include <Explorer/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox :public Explorer::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());	//ÃÌº” æ¿˝µΩ≤„’ª
		PushLayer(new Sandbox2D());		//ÃÌº”2D‰÷»æ≤‚ ‘≤„
	}

	~Sandbox()
	{

	}
};

Explorer::Application* Explorer::CreateApplication()
{
	return new Sandbox();
}