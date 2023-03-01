#pragma once

#include "Explorer/Core/Core.h"

namespace Explorer
{
	/// <summary>
	/// 帧缓冲区规范
	/// </summary>
	struct FramebufferSpecification
	{
		uint32_t Width;		//帧缓冲区宽
		uint32_t Height;	//帧缓冲区高

		uint32_t Samples = 1;

		bool SwapChainTarget = false;	//是否要渲染到屏幕
	};

	/// <summary>
	/// 帧缓冲区
	/// </summary>
	class Framebuffer
	{
	private:
		uint32_t m_RendererID = 0;					//帧缓冲区ID
		uint32_t m_ColorAttachment = 0;				//颜色缓冲区
		uint32_t m_DepthAttachment = 0;				//深度缓冲区
		FramebufferSpecification m_Specification;	//帧缓冲区规范
	public:
		/// <summary>
		/// 帧缓冲区
		/// </summary>
		/// <param name="spec">帧缓冲区规范</param>
		Framebuffer(const FramebufferSpecification& spec);
		~Framebuffer();

		/// <summary>
		/// 调整大小：使无效
		/// </summary>
		void Invalidate();

		/// <summary>
		/// 绑定帧缓冲区
		/// </summary>
		void Bind();

		/// <summary>
		/// 解除绑定
		/// </summary>
		void Unbind();

		/// <summary>
		/// 重置帧缓冲区大小
		/// </summary>
		/// <param name="width">宽</param>
		/// <param name="height">高</param>
		void Resize(uint32_t width, uint32_t height);

		/// <summary>
		/// 返回颜色缓冲区ID
		/// </summary>
		/// <returns>颜色缓冲区ID</returns>
		uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }

		/// <summary>
		/// 返回帧缓冲区规范
		/// </summary>
		/// <returns>帧缓冲区规范</returns>
		const FramebufferSpecification& GetSpecification() const { return m_Specification; }
	};
}