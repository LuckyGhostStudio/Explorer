#pragma once

/// <summary>
/// ����Ӧ�ó�����SandboxApp����
/// </summary>
/// <returns>Ӧ�ó���ָ��</returns>
extern Explorer::Application* Explorer::CreateApplication();

int main(int argc, char** argv)
{
	Explorer::Log::Init();
	EXP_CORE_WARN("��ʼ����־��");

	EXP_INFO("Hello��Explorer!");

	auto app = Explorer::CreateApplication();	//����Ӧ�ó���
	app->Run();		//����
	delete app;
}