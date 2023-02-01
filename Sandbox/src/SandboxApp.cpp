#include <Explorer.h>

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