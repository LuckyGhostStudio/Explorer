#include "exppch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Platforms/OpenGL/ImGuiOpenGLRenderer.h"
#include "GLFW/glfw3.h"

#include "Explorer/Application.h"

namespace Explorer
{
	ImGuiLayer::ImGuiLayer() :Layer("ImGuiLayer")
	{

	}

	ImGuiLayer::~ImGuiLayer()
	{

	}

	void ImGuiLayer::OnAttach()
	{
		ImGui::CreateContext();		//创建ImGui上下文
		ImGui::StyleColorsDark();	//深色样式

		ImGuiIO& io = ImGui::GetIO();
		io.BackendFlags |= ImGuiBackendFlags_HasMouseCursors;	//可以接受GetMouseCursor()的值
		io.BackendFlags |= ImGuiBackendFlags_HasSetMousePos;	//可以接受io.WantSetMousePos请求

		//临时：最终将使用Explorer按键代码
		io.KeyMap[ImGuiKey_Tab] = GLFW_KEY_TAB;
		io.KeyMap[ImGuiKey_LeftArrow] = GLFW_KEY_LEFT;
		io.KeyMap[ImGuiKey_RightArrow] = GLFW_KEY_RIGHT;
		io.KeyMap[ImGuiKey_UpArrow] = GLFW_KEY_UP;
		io.KeyMap[ImGuiKey_DownArrow] = GLFW_KEY_DOWN;
		io.KeyMap[ImGuiKey_PageUp] = GLFW_KEY_PAGE_UP;
		io.KeyMap[ImGuiKey_PageDown] = GLFW_KEY_PAGE_DOWN;
		io.KeyMap[ImGuiKey_Home] = GLFW_KEY_HOME;
		io.KeyMap[ImGuiKey_End] = GLFW_KEY_END;
		io.KeyMap[ImGuiKey_Insert] = GLFW_KEY_INSERT;
		io.KeyMap[ImGuiKey_Delete] = GLFW_KEY_DELETE;
		io.KeyMap[ImGuiKey_Backspace] = GLFW_KEY_BACKSPACE;
		io.KeyMap[ImGuiKey_Space] = GLFW_KEY_SPACE;
		io.KeyMap[ImGuiKey_Enter] = GLFW_KEY_ENTER;
		io.KeyMap[ImGuiKey_Escape] = GLFW_KEY_ESCAPE;
		io.KeyMap[ImGuiKey_A] = GLFW_KEY_A;
		io.KeyMap[ImGuiKey_C] = GLFW_KEY_C;
		io.KeyMap[ImGuiKey_V] = GLFW_KEY_V;
		io.KeyMap[ImGuiKey_X] = GLFW_KEY_X;
		io.KeyMap[ImGuiKey_Y] = GLFW_KEY_Y;
		io.KeyMap[ImGuiKey_Z] = GLFW_KEY_Z;

		ImGui_ImplOpenGL3_Init("#version 410");	//初始化ImGui
	}

	void ImGuiLayer::OnDetach()
	{

	}

	void ImGuiLayer::OnUpdate()
	{
		//设置io的显示尺寸
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::GetInstance();
		io.DisplaySize = ImVec2(app.GetWindow().GetWidth(), app.GetWindow().GetHeight());	//io的显示显示尺寸
		
		//计算io帧间隔
		float time = (float)glfwGetTime();	//当前时间
		io.DeltaTime = m_Time > 0.0f ? (time - m_Time) : (1.0f / 60.0f);	//帧间隔时间
		m_Time = time;

		//开启一个新的帧
		ImGui_ImplOpenGL3_NewFrame();
		ImGui::NewFrame();

		static bool show = true;
		ImGui::ShowDemoWindow(&show);	//显示Demo窗口

		ImGui::Render();	//渲染ImGui

		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());	//渲染绘制数据
	}

	void ImGuiLayer::OnEvent(Event& event)
	{

	}
}