#include "exppch.h"
#include "Renderer.h"
#include "Renderer3D.h"

namespace Explorer
{
	Renderer::Type Renderer::s_Type = Renderer::Type::Rasterization;	//光栅化渲染器

	void Renderer::Init()
	{
		RenderCommand::Init();
		Renderer3D::Init();
	}

	void Renderer::Shutdown()
	{
		Renderer3D::Shutdown();
	}

	void Renderer::OnWindowResize(uint32_t width, uint32_t height)
	{
		RenderCommand::SetViewport(0, 0, width, height);	//设置视口大小
	}

	void Renderer::BeginScene(Camera& camera)
	{
		
	}

	void Renderer::EndScene()
	{

	}
}