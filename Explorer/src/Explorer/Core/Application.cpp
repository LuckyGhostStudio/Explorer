#include "exppch.h"	//Ԥ����ͷ�ļ�
#include "Application.h"

#include "Explorer/Core/Log.h"
#include "Input.h"

#include "Explorer/Renderer/Renderer.h"

#include <GLFW/glfw3.h>

namespace Explorer
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)		//�󶨺���x ���غ�������

	Application* Application::Instance = nullptr;	//����

	Application::Application()
	{
		EXP_CORE_ASSERT(!Instance, "Application already exisit!");	//Application�Ѵ���
		Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());	//��������
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));		//���ûص�����

		Renderer::Init();	//��ʼ����Ⱦ��

		m_ImGuiLayer = new ImGuiLayer();		//����ImGui��
		PushOverlay(m_ImGuiLayer);				//���ImGuiLayer�����ǲ�
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();	//��ʼ����
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);	//�¼�������
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));	//���� ���ڹر��¼�
		dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(OnWindowResize));	//���� ���������¼�

		//��������±�����ջ
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);		//���ȡ�������¼�
			if (e.GetHandled()) break;	//�¼��Ѵ��� ����
		}
	}

	void Application::Run()
	{
		while (m_Running) {
			float time = (float)glfwGetTime();				//��ǰʱ��
			DeltaTime deltaTime = time - m_LastFrameTime;	//֡��� = ��ǰʱ�� - ��һ֡ʱ��
			m_LastFrameTime = time;							//������һ֡ʱ��

			if (!m_Minimized) {		//����δ��С��
				//���²�ջ�����в�
				for (Layer* layer : m_LayerStack) {
					layer->OnUpdate(deltaTime);
				}

				//ImGui��Ⱦ
				m_ImGuiLayer->Begin();
				for (Layer* layer : m_LayerStack) {
					layer->OnImGuiRender();		//��Ⱦÿ��Laye��ImGui
				}
				m_ImGuiLayer->End();
			}

			m_Window->OnUpdate();		//��OnUpdate����ѯ������յ��¼�
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;	//��������
		return true;
	}

	bool Application::OnWindowResize(WindowResizeEvent& e)
	{
		if (e.GetWidth() == 0 || e.GetHeight() == 0) {
			m_Minimized = true;		//������С��
			return false;
		}

		m_Minimized = false;
		Renderer::OnWindowResize(e.GetWidth(), e.GetHeight());	//���д������ź������

		return false;
	}
}