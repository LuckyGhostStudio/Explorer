#include <Explorer.h>
#include <Explorer/Core/EntryPoint.h>

#include "Sandbox3D.h"

class Sandbox :public Explorer::Application
{
public:
	Sandbox()
	{
		PushLayer(new Sandbox3D());		//���3D��Ⱦ���Բ�
	}

	~Sandbox()
	{

	}
};

Explorer::Application* Explorer::CreateApplication()
{
	return new Sandbox();
}