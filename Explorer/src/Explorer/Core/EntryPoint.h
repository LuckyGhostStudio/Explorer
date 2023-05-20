#pragma once

/// <summary>
/// 创建应用程序：在SandboxApp定义
/// </summary>
/// <returns>应用程序指针</returns>
extern Explorer::Application* Explorer::CreateApplication();

int main(int argc, char** argv)
{
	Explorer::Log::Init();
	EXP_CORE_WARN("初始化日志！");

	EXP_INFO("Hello，Explorer!");

	auto app = Explorer::CreateApplication();	//创建应用程序
	app->Run();		//运行
	delete app;
}