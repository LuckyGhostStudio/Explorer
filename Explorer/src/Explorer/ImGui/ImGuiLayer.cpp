#include "exppch.h"
#include "ImGuiLayer.h"

#include "imgui.h"
#include "Explorer/Core/Application.h"

#include "examples/imgui_impl_opengl3.h"
#include "examples/imgui_impl_glfw.h" 

#include <GLFW/glfw3.h>
#include <glad/glad.h>

#include "ImGuizmo.h"

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
		//����ImGui������
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;       //���ü��̿���
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;           //����ͣ���ռ�
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;         //���ö��ӿ�/ƽ̨����

		float fontSize = 18.0f;
		io.Fonts->AddFontFromFileTTF("Resources/Fonts/opensans/OpenSans-Bold.ttf", fontSize);	//��Ӵ��壨0�ţ�
		//Ĭ������ ���TTF���� ��С18
		io.FontDefault = io.Fonts->AddFontFromFileTTF("Resources/Fonts/opensans/OpenSans-Regular.ttf", fontSize);	//1��

		//����ImGui��ʽ
		ImGui::StyleColorsDark();

		// When viewports are enabled we tweak WindowRounding/WindowBg so platform windows can look identical to regular ones.
		ImGuiStyle& style = ImGui::GetStyle();
		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			style.WindowRounding = 0.0f;
			style.Colors[ImGuiCol_WindowBg].w = 1.0f;
		}

		SetDarkThemeColors();	//������ɫ����

		Application& app = Application::GetInstance();
		GLFWwindow* window = static_cast<GLFWwindow*>(app.GetWindow().GetNativeWindow());

		//����ƽ̨/��Ⱦ����
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init("#version 410");
	}

	void ImGuiLayer::OnDetach()
	{
		//�ر�ImGui ����������
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}

	void ImGuiLayer::OnEvent(Event& e)
	{
		//��ֹ�����¼�
		if (m_BlockEvents) {
			ImGuiIO& io = ImGui::GetIO();
			e.m_Handled |= e.IsInCategory(EventCategoryMouse) & io.WantCaptureMouse;		//��������¼�
			e.m_Handled |= e.IsInCategory(EventCategoryKeyboard) & io.WantCaptureKeyboard;	//��������¼�
		}
	}

	void ImGuiLayer::Begin()
	{
		//������֡
		ImGui_ImplOpenGL3_NewFrame();
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame(); //������֡
	}

	void ImGuiLayer::End()
	{
		ImGuiIO& io = ImGui::GetIO();
		Application& app = Application::GetInstance();
		io.DisplaySize = ImVec2((float)app.GetWindow().GetWidth(), (float)app.GetWindow().GetHeight());	//���ô��ڴ�С

		//��ȾImGui
		ImGui::Render();
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
	}

	void ImGuiLayer::SetDarkThemeColors()
	{
		auto& colors = ImGui::GetStyle().Colors;	//��ɫ��ʽ

		//���ڱ���-
		colors[ImGuiCol_WindowBg] = ImVec4{ 0.2196f, 0.2196f, 0.2196f, 1.0f };

		//Header-
		colors[ImGuiCol_Header] = ImVec4{ 0.2431f, 0.2431f, 0.2431f, 1.0f };
		colors[ImGuiCol_HeaderHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_HeaderActive] = ImVec4{ 0.2431f, 0.2431f, 0.2431f, 1.0f };

		//��ť-
		colors[ImGuiCol_Button] = ImVec4{ 0.3451f, 0.3451f, 0.3451f, 1.0f };
		colors[ImGuiCol_ButtonHovered] = ImVec4{ 0.3882f, 0.3882f, 0.3882f, 1.0f };
		colors[ImGuiCol_ButtonActive] = ImVec4{ 0.3451f, 0.3451f, 0.3451f, 1.0f };

		//�ؼ�����-
		colors[ImGuiCol_FrameBg] = ImVec4{ 0.1569f, 0.1569f, 0.1569f, 1.0f };
		colors[ImGuiCol_FrameBgHovered] = ImVec4{ 0.3f, 0.305f, 0.31f, 1.0f };
		colors[ImGuiCol_FrameBgActive & ImGuiCol_Border] = ImVec4{ 0.498f, 0.8392f, 0.9921f, 1.0f };

		//��ǩ
		colors[ImGuiCol_Tab] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabHovered] = ImVec4{ 0.38f, 0.3805f, 0.381f, 1.0f };
		colors[ImGuiCol_TabActive] = ImVec4{ 0.28f, 0.2805f, 0.281f, 1.0f };
		colors[ImGuiCol_TabUnfocused] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4{ 0.2f, 0.205f, 0.21f, 1.0f };

		//����-
		colors[ImGuiCol_TitleBg] = ImVec4{ 0.1569f, 0.1569f, 0.1569f, 1.0f };
		colors[ImGuiCol_TitleBgActive] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4{ 0.15f, 0.1505f, 0.151f, 1.0f };

		//�ؼ��߽���ɫ-
		colors[ImGuiCol_Border] = ImVec4{ 0.1647f, 0.1647f, 0.1647f, 1.0f };

		//�ı���ɫ
		colors[ImGuiCol_Text] = ImVec4{ 0.95f, 0.95f, 0.95f, 1.0f };
		colors[ImGuiCol_TextDisabled] = ImVec4{ 0.5f, 0.5f, 0.5f, 1.0f };	//����ʱ��ɫ

		//�����˵���ɫ-
		colors[ImGuiCol_PopupBg] = ImVec4{ 0.2196f, 0.2196f, 0.2196f, 1.0f };

		//��ѡ��Ժ���ɫ-
		colors[ImGuiCol_CheckMark] = ImVec4{ 0.8863f, 0.8863f, 0.8863f, 1.0f };

		colors[ImGuiCol_MenuBarBg] = ImVec4{ 0.2196f, 0.2196f, 0.2196f, 1.0f };
		colors[ImGuiCol_ScrollbarBg] = ImVec4{ 0.2078f, 0.2078f, 0.2078f, 1.0f };
		colors[ImGuiCol_ScrollbarGrab] = ImVec4{ 0.3725f, 0.3725f, 0.3725f, 1.0f };
		colors[ImGuiCol_SliderGrab] = ImVec4{ 0.6f, 0.6f, 0.6f, 1.0f };
		colors[ImGuiCol_Separator] = ImVec4{ 0.098f, 0.098f, 0.098f, 1.0f };
	}
}