#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <chrono>

/// <summary>
/// 计时器
/// </summary>
/// <typeparam name="Fn">计时器停止时回调函数</typeparam>
template<typename Fn>
class Timer
{
public:
	Timer(const char* name, Fn&& func)
		: m_Name(name), m_Func(func), m_Stopped(false)
	{
		m_StartTimepoint = std::chrono::high_resolution_clock::now();	//开始时间点
	}

	~Timer()
	{
		if (!m_Stopped)
			Stop();
	}

	void Stop()
	{
		auto endTimepoint = std::chrono::high_resolution_clock::now();	//结束时间点

		//开始结束时间（微妙）
		long long start = std::chrono::time_point_cast<std::chrono::microseconds>(m_StartTimepoint).time_since_epoch().count();
		long long end = std::chrono::time_point_cast<std::chrono::microseconds>(endTimepoint).time_since_epoch().count();

		m_Stopped = true;

		float duration = (end - start) * 0.001f;	//持续时间（毫秒）
		m_Func({ m_Name, duration });				//将测试结果添加到列表
	}
private:
	const char* m_Name;		//测试程序段名
	Fn m_Func;
	std::chrono::time_point<std::chrono::steady_clock> m_StartTimepoint;	//测试开始时间点
	bool m_Stopped;
};

//测试name程序段运行用时 并将测试结果profileResult添加到列表m_ProfileResults
#define PROFILE_SCOPE(name) Timer timer##__LINE__(name, [&](ProfileResult profileResult) { m_ProfileResults.push_back(profileResult); })


Sandbox2D::Sandbox2D() :Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f) {}

void Sandbox2D::OnAttach()
{
	m_CheckerboardTexture = std::make_shared<Explorer::Texture2D>("asserts/textures/Checkerboard.png");	//创建纹理
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Explorer::DeltaTime dt)
{
	PROFILE_SCOPE("Sandbox2D::OnUpdate");

	{
		PROFILE_SCOPE("CameraController::OnUpdate");
		m_CameraController.OnUpdate(dt);	//更新相机控制器
	}

	//Renderer
	{
		PROFILE_SCOPE("Renderer Preparation");
		Explorer::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });	//设置清屏颜色
		Explorer::RenderCommand::Clear();									//清除
	}

	{
		PROFILE_SCOPE("Renderer Draw");
		Explorer::Renderer2D::BeginScene(m_CameraController.GetCamera());		//开始渲染场景

		Explorer::Renderer2D::DrawQuad(m_SquarePosition, m_SquareRotation.z, m_SquareScale, m_SquareColor, m_CheckerboardTexture);	//绘制四边形
		Explorer::Renderer2D::DrawQuad({ -1.0f, 0.0f }, 0.0f, { 0.8f, 0.8f }, { 0.8f, 0.2f, 0.3f, 1.0f });	//绘制四边形
		Explorer::Renderer2D::DrawQuad({ 0.5f, -0.5f }, 0.0f, { 0.5f, 0.75f }, { 0.2f, 0.8f, 0.3f, 1.0f });	//绘制四边形

		Explorer::Renderer2D::EndScene();						//结束渲染场景
	}
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Color", glm::value_ptr(m_SquareColor));	//颜色编辑UI

	ImGui::SliderFloat3("Position", glm::value_ptr(m_SquarePosition), -10.0f, 10.0f);
	ImGui::SliderFloat3("Rotation", glm::value_ptr(m_SquareRotation), -360.0f, 360.0f);
	ImGui::SliderFloat3("Scale", glm::value_ptr(m_SquareScale), 0.0f, 10.0f);

	ImGui::End();

	//性能分析
	ImGui::Begin("Profiling");
	for (auto& result : m_ProfileResults) {
		char label[50];
		strcpy(label, "%.3fms  ");
		strcat(label, result.Name);
		ImGui::Text(label, result.Time);	//显示性能测试结果（测试程序段名 运行时间）
	}
	m_ProfileResults.clear();

	ImGui::End();
}

void Sandbox2D::OnEvent(Explorer::Event& event)
{
	m_CameraController.OnEvent(event);	//调用相机事件函数
}