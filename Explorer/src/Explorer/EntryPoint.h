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
	int a = 2;
	EXP_INFO("��ã�{0}", a);

	auto app = Explorer::CreateApplication();	//����Ӧ�ó���
	app->Run();		//����
	delete app;
}

#endif // EXP_PLATFORM_WINDOWS