#include <Explorer.h>
#include <Explorer/Core/EntryPoint.h>

class Sandbox :public Explorer::Application
{
public:
	Sandbox()
	{
		
	}

	~Sandbox()
	{

	}
};

Explorer::Application* Explorer::CreateApplication()
{
	return new Sandbox();
}