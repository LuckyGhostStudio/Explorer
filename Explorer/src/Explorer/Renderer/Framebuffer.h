#pragma once

#include "Explorer/Core/Core.h"

namespace Explorer
{
	/// <summary>
	/// 帧缓冲区纹理格式
	/// </summary>
	enum class FramebufferTextureFormat
	{
		None = 0,

		RGBA8,				//颜色
		RED_INTEGER,

		DEFPTH24STENCIL8,	//深度模板

		Depth = DEFPTH24STENCIL8	//默认值
	};

	/// <summary>
	/// 帧缓冲区纹理规范
	/// </summary>
	struct FramebufferTextureSpecification
	{
		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;	//纹理格式

		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format) :TextureFormat(format) {}
	};

	/// <summary>
	/// 帧缓冲区附件规范
	/// </summary>
	struct FramebufferAttachmentSpecification
	{
		std::vector<FramebufferTextureSpecification> Attachments;	//附件列表

		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			:Attachments(attachments) {}
	};

	/// <summary>
	/// 帧缓冲区规范
	/// </summary>
	struct FramebufferSpecification
	{
		uint32_t Width;		//帧缓冲区宽
		uint32_t Height;	//帧缓冲区高
		FramebufferAttachmentSpecification Attachments;	//帧缓冲区所有附件
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
		FramebufferSpecification m_Specification;	//帧缓冲区规范
		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;					//颜色缓冲区规范列表
		FramebufferTextureSpecification m_DepthAttachmentSpec = FramebufferTextureFormat::None;	//深度缓冲区规范

		std::vector<uint32_t> m_ColorAttachments;	//颜色缓冲区ID列表
		uint32_t m_DepthAttachment = 0;				//深度缓冲区ID
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
		/// 读取像素
		/// </summary>
		/// <param name="attachmentIndex">颜色缓冲区id</param>
		/// <param name="x">x坐标</param>
		/// <param name="y">y坐标</param>
		/// <returns>像素数据：输出到attachmentIndex颜色缓冲区的数据</returns>
		int ReadPixel(uint32_t attachmentIndex, int x, int y);

		/// <summary>
		/// 清除帧缓冲区附件
		/// </summary>
		/// <param name="attachmentIndex">颜色缓冲区id</param>
		/// <param name="value">清除值</param>
		void ClearAttachment(uint32_t attachmentIndex, int value);

		/// <summary>
		/// 返回颜色缓冲区ID
		/// </summary>
		/// <param name="index">id列表索引</param>
		/// <returns>颜色缓冲区ID</returns>
		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const
		{
			EXP_CORE_ASSERT(index < m_ColorAttachments.size(), "index 越界！");
			return m_ColorAttachments[index];
		}

		/// <summary>
		/// 返回帧缓冲区规范
		/// </summary>
		/// <returns>帧缓冲区规范</returns>
		const FramebufferSpecification& GetSpecification() const { return m_Specification; }
	};
}