#include <Explorer.h>
#include <Explorer/Core/EntryPoint.h>

#include "Sandbox3D.h"

class Sandbox :public Explorer::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox3D());		//ÃÌº”3D‰÷»æ≤‚ ‘≤„
	}

	~Sandbox()
	{

	}
};

Explorer::Application* Explorer::CreateApplication()
{
	return new Sandbox();
}