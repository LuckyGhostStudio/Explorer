#include "exppch.h"	//预编译头文件
#include "Application.h"

#include "Explorer/Log.h"
#include "Input.h"

#include <glad/glad.h>

namespace Explorer
{
#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)		//绑定函数x 返回函数对象

	Application* Application::Instance = nullptr;	//单例

	/// <summary>
	/// ShaderDataType转换为OpenGL基本类型
	/// </summary>
	/// <param name="">ShaderDataType</param>
	/// <returns>OpenGL基本类型</returns>
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
		case Explorer::ShaderDataType::Float:	return GL_FLOAT;
		case Explorer::ShaderDataType::Float2:	return GL_FLOAT;
		case Explorer::ShaderDataType::Float3:	return GL_FLOAT;
		case Explorer::ShaderDataType::Float4:	return GL_FLOAT;
		case Explorer::ShaderDataType::Mat3:	return GL_FLOAT;
		case Explorer::ShaderDataType::Mat4:	return GL_FLOAT;
		case Explorer::ShaderDataType::Int:		return GL_INT;
		case Explorer::ShaderDataType::Int2:	return GL_INT;
		case Explorer::ShaderDataType::Int3:	return GL_INT;
		case Explorer::ShaderDataType::Int4:	return GL_INT;
		case Explorer::ShaderDataType::Bool:	return GL_BOOL;
		}

		EXP_CORE_ASSERT(false, "Unknow ShaderDataType!");
		return 0;
	}

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

		float vertices[] = {
			//------位置------   ---------颜色---------
			-0.5f, -0.5f, 0.0f,	1.0f, 0.0f, 1.0f, 1.0f,	//左下
			 0.5f, -0.5f, 0.0f,	0.0f, 1.0f, 1.0f, 1.0f,	//右下
			 0.0f,  0.5f, 0.0f,	1.0f, 1.0f, 0.0f, 1.0f,	//上
		};

		m_VertexBuffer.reset(new VertexBuffer(vertices, sizeof(vertices)));		//创建顶点缓冲

		{
			//顶点缓冲区布局（出作用域销毁）
			BufferLayout layout = {
				{ShaderDataType::Float3, "a_Position"},	//位置
				{ShaderDataType::Float4, "a_Color"}		//颜色
			};

			m_VertexBuffer->SetLayout(layout);	//设置顶点缓冲区布局
		}

		const auto& layout = m_VertexBuffer->GetLayout();		//顶点缓冲区布局

		uint32_t index = 0;
		for (const auto& element : layout) {
			glEnableVertexAttribArray(index);					//启用顶点属性

			glVertexAttribPointer(index,						//顶点属性位置编号
				element.GetComponentCount(),					//顶点属性数据个数
				ShaderDataTypeToOpenGLBaseType(element.Type), 	//数据类型
				element.Normalized ? GL_TRUE : GL_FALSE, 		//是否标准化
				layout.GetStride(), 							//顶点大小（字节）
				(const void*)element.Offset);					//顶点属性偏移量（字节）

			index++;
		}

		unsigned int indices[3] = { 0,1,2 };	//顶点索引

		m_IndexBuffer.reset(new IndexBuffer(indices, sizeof(indices) / sizeof(uint32_t)));		//创建索引缓冲

		std::string vertexSrc = R"(
			#version 330 core
			
			layout(location = 0) in vec3 a_Position;
			layout(location = 1) in vec4 a_Color;

			out vec3 v_Position;			
			out vec4 v_Color;			

			void main()
			{
				v_Position = a_Position;
				v_Color = a_Color;
				gl_Position = vec4(a_Position, 1.0);
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