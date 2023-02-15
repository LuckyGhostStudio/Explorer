#include "exppch.h"	//Ԥ����ͷ�ļ�
#include "Application.h"

#include "Explorer/Log.h"
#include "Input.h"

#include <glad/glad.h>

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

		m_ImGuiLayer = new ImGuiLayer();		//����ImGui��
		PushOverlay(m_ImGuiLayer);				//���ImGuiLayer�����ǲ�

		glGenVertexArrays(1, &m_VertexArray);	//������������
		glBindVertexArray(m_VertexArray);		//��

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,		//����
			 0.5f, -0.5f, 0.0f,		//����
			 0.0f,  0.5f, 0.0f		//��
		};

		m_VertexBuffer.reset(new VertexBuffer(vertices, sizeof(vertices)));		//�������㻺��

		glEnableVertexAttribArray(0);	//����0�Ŷ�������
		//0�Ŷ������� ÿ������3������ float���� ÿ������ռ�ֽ��� λ��������һ�������е�ƫ����
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);	//���ö������Բ��֣�����λ��

		unsigned int indices[3] = { 0,1,2 };	//��������

		m_IndexBuffer.reset(new IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));		//������������

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;

			out vec3 v_Position;			

			void main()
			{
				v_Position = a_Position;
				gl_Position = vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			
			void main()
			{
				color = vec4(v_Position * 0.5 + 0.5, 1.0);
			}
		)";

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));	//������ɫ��
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

		//��������±�����ջ
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);		//���ȡ�������¼�
			if (e.GetHandled()) break;	//�¼��Ѵ��� ����
		}
	}

	void Application::Run()
	{
		while (m_Running) {
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);							//��VAO
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);	//������������������

			//���²�ջ�����в�
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			//ImGui��Ⱦ
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();		//��Ⱦÿ��Laye��ImGui
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();		//��OnUpdate����ѯ������յ��¼�
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;	//��������
		return true;
	}
}