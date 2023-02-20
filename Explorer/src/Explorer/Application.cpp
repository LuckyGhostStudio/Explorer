#include "exppch.h"	//预编译头文件
#include "Application.h"

#include "Explorer/Log.h"
#include "Input.h"

#include "Renderer/Renderer.h"

namespace Explorer
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)		//绑定函数x 返回函数对象

	Application* Application::Instance = nullptr;	//单例

	Application::Application() :m_Camera(-1.6f, 1.6f, -0.9f, 0.9f)
	{
		EXP_CORE_ASSERT(!Instance, "Application already exisit!");	//Application已存在
		Instance = this;

		m_Window = std::unique_ptr<Window>(Window::Create());	//创建窗口
		m_Window->SetEventCallback(BIND_EVENT_FN(OnEvent));		//设置回调函数

		m_ImGuiLayer = new ImGuiLayer();		//创建ImGui层
		PushOverlay(m_ImGuiLayer);				//添加ImGuiLayer到覆盖层

		m_VertexArray.reset(new VertexArray());		//创建顶点数组对象

		float vertices[] = {
			//------位置------   ---------颜色---------
			-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,	//左下
			 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 1.0f, 1.0f,	//右下
			 0.0f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f,	//上
		};

		std::shared_ptr<VertexBuffer> vertexBuffer;								//VBO
		vertexBuffer.reset(new VertexBuffer(vertices, sizeof(vertices)));		//创建顶点缓冲

		//顶点缓冲区布局
		BufferLayout layout = {
			{ShaderDataType::Float3, "a_Position"},	//位置
			{ShaderDataType::Float4, "a_Color"}		//颜色
		};

		vertexBuffer->SetLayout(layout);				//设置顶点缓冲区布局
		m_VertexArray->AddVertexBuffer(vertexBuffer);	//添加VBO到VAO

		unsigned int indices[3] = { 0,1,2 };	//顶点索引

		std::shared_ptr<IndexBuffer> indexBuffer;												//EBO
		indexBuffer.reset(new IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));		//创建索引缓冲
		m_VertexArray->SetIndexBuffer(indexBuffer);	//设置EBO到VAO

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			uniform mat4 u_ViewProjectionMatrix;

			out vec3 v_Position;			
			out vec4 v_Color;			

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = u_ViewProjectionMatrix * vec4(a_Position, 1.0);
			}
		)";

		std::string fragmentSrc = R"(
			#version 330 core
			
			layout(location = 0) out vec4 color;

			in vec3 v_Position;
			in vec4 v_Color;
			
			void main()
			{
				color = v_Color;
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
		float angle = 0.0f;

		while (m_Running) {
			RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });
			RenderCommand::Clear();

			m_Camera.SetPosition({ 0.5f, 0.5f, 0.0f });
			m_Camera.SetRotation(angle += 0.2f);

			Renderer::BeginScene(m_Camera);				//开始渲染场景
			Renderer::Submit(m_Shader, m_VertexArray);	//提交渲染命令
			Renderer::EndScene();						//结束渲染场景
			
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