#include "Sandbox2D.h"
#include "imgui/imgui.h"

#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

Sandbox2D::Sandbox2D() :Layer("Sandbox2D"), m_CameraController(1280.0f / 720.0f)
{

}

void Sandbox2D::OnAttach()
{
	m_SquareVA = std::make_shared<Explorer::VertexArray>();		//创建顶点数组对象

	float squareVertices[] = {
		//------位置------  
		-0.5f, -0.5f, 0.0f,	//左下0
		 0.5f, -0.5f, 0.0f,	//右下1
		-0.5f,  0.5f, 0.0f,	//左上2
		 0.5f,  0.5f, 0.0f  //右上3
	};

	std::shared_ptr<Explorer::VertexBuffer> squareVB;									//VBO
	squareVB.reset(new Explorer::VertexBuffer(squareVertices, sizeof(squareVertices)));	//创建顶点缓冲

	//设置顶点缓冲区布局
	squareVB->SetLayout({
		{Explorer::ShaderDataType::Float3, "a_Position"},	//位置
		});
	m_SquareVA->AddVertexBuffer(squareVB);	//添加VBO到VAO

	unsigned int squareIndices[] = { 0, 1, 2, 2, 1, 3 };		//顶点索引

	std::shared_ptr<Explorer::IndexBuffer> squareIB;														//EBO
	squareIB.reset(new Explorer::IndexBuffer(squareIndices, sizeof(squareIndices) / sizeof(uint32_t)));		//创建索引缓冲
	m_SquareVA->SetIndexBuffer(squareIB);																	//设置EBO到VAO

	m_FlatColorShader = std::make_shared<Explorer::Shader>("asserts/shaders/FlatColorShader");				//创建着色器
}

void Sandbox2D::OnDetach()
{

}

void Sandbox2D::OnUpdate(Explorer::DeltaTime dt)
{
	m_CameraController.OnUpdate(dt);	//更新相机控制器

	//Renderer
	Explorer::RenderCommand::SetClearColor({ 0.1f, 0.1f, 0.1f, 1.0f });	//设置清屏颜色
	Explorer::RenderCommand::Clear();									//清除

	Explorer::Renderer::BeginScene(m_CameraController.GetCamera());		//开始渲染场景

	m_FlatColorShader->Bind();
	m_FlatColorShader->UploadUniformFloat4("u_Color", m_SquareColor);	//上传uniform变量

	Explorer::Renderer::Submit(m_FlatColorShader, m_SquareVA, glm::scale(glm::mat4(1.0f), glm::vec3(1.5f)));	//提交渲染指令

	Explorer::Renderer::EndScene();						//结束渲染场景
}

void Sandbox2D::OnImGuiRender()
{
	ImGui::Begin("Settings");
	ImGui::ColorEdit4("Square Color", glm::value_ptr(m_SquareColor));	//颜色编辑UI
	ImGui::End();
}

void Sandbox2D::OnEvent(Explorer::Event& event)
{
	m_CameraController.OnEvent(event);	//调用相机事件函数
}