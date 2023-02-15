#include "exppch.h"	//预编译头文件
#include "Application.h"

#include "Explorer/Log.h"
#include "Input.h"

#include <glad/glad.h>

namespace Explorer
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)		//绑定函数x 返回函数对象

	Application* Application::Instance = nullptr;	//单例

	Application::Application()
	{
		EXP_CORE_ASSERT(!Instance, "Application already exisit!");	//Application已存在
		Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());	//创建窗口
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));		//设置回调函数

		m_ImGuiLayer = new ImGuiLayer();		//创建ImGui层
		PushOverlay(m_ImGuiLayer);				//添加ImGuiLayer到覆盖层

		glGenVertexArrays(1, &m_VertexArray);	//创建顶点数组
		glBindVertexArray(m_VertexArray);		//绑定

		float vertices[3 * 3] = {
			-0.5f, -0.5f, 0.0f,		//左下
			 0.5f, -0.5f, 0.0f,		//右下
			 0.0f,  0.5f, 0.0f		//上
		};

		m_VertexBuffer.reset(new VertexBuffer(vertices, sizeof(vertices)));		//创建顶点缓冲

		glEnableVertexAttribArray(0);	//启用0号顶点属性
		//0号顶点属性 每个顶点3个数据 float类型 每个顶点占字节数 位置属性在一个顶点中的偏移量
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), nullptr);	//设置顶点属性布局：顶点位置

		unsigned int indices[3] = { 0,1,2 };	//顶点索引

		m_IndexBuffer.reset(new IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));		//创建索引缓冲

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

		m_Shader.reset(new Shader(vertexSrc, fragmentSrc));	//创建着色器
	}

	Application::~Application()
	{

	}

	void Application::PushLayer(Layer* layer)
	{
		m_LayerStack.PushLayer(layer);
		layer->OnAttach();	//初始化层
	}

	void Application::PushOverlay(Layer* layer)
	{
		m_LayerStack.PushOverlay(layer);
		layer->OnAttach();
	}

	void Application::OnEvent(Event& e)
	{
		EventDispatcher dispatcher(e);	//事件调度器
		dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));	//调度 窗口关闭事件

		//从最顶层向下遍历层栈
		for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();) {
			(*--it)->OnEvent(e);		//层获取并处理事件
			if (e.GetHandled()) break;	//事件已处理 跳出
		}
	}

	void Application::Run()
	{
		while (m_Running) {
			glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
			glClear(GL_COLOR_BUFFER_BIT);

			m_Shader->Bind();
			glBindVertexArray(m_VertexArray);							//绑定VAO
			glDrawElements(GL_TRIANGLES, m_IndexBuffer->GetCount(), GL_UNSIGNED_INT, nullptr);	//根据索引绘制三角形

			//更新层栈中所有层
			for (Layer* layer : m_LayerStack) {
				layer->OnUpdate();
			}

			//ImGui渲染
			m_ImGuiLayer->Begin();
			for (Layer* layer : m_LayerStack) {
				layer->OnImGuiRender();		//渲染每个Laye的ImGui
			}
			m_ImGuiLayer->End();

			m_Window->OnUpdate();		//在OnUpdate中轮询处理接收的事件
		}
	}

	bool Application::OnWindowClose(WindowCloseEvent& e)
	{
		m_Running = false;	//结束运行
		return true;
	}
}