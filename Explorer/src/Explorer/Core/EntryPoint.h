#pragma once

#ifdef EXP_PLATFORM_WINDOWS

/// <summary>
/// ����Ӧ�ó�����SandboxApp����
/// </summary>
/// <returns>Ӧ�ó���ָ��</returns>
extern Explorer::Application* Explorer::CreateApplication();

int main(int argc, char** argv)
{
	Explorer::Log::Init();
	EXP_CORE_WARN("��ʼ����־��");

	EXP_INFO("��ã�Explorer!");

	auto app = Explorer::CreateApplication();	//����Ӧ�ó���
	app->Run();		//����
	delete app;
}

#endif // EXP_PLATFORM_WINDOWS