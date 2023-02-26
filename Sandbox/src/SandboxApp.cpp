#include <Explorer.h>
#include <Explorer/Core/EntryPoint.h>

#include "Sandbox2D.h"

class Sandbox :public Explorer::Application
{
public:
	Sandbox()
	{
		//PushLayer(new ExampleLayer());	//���ʾ������ջ
		PushLayer(new Sandbox2D());		//���2D��Ⱦ���Բ�
	}

	~Sandbox()
	{

	}
};

Explorer::Application* Explorer::CreateApplication()
{
	return new Sandbox();
}