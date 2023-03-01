#pragma once

#include "Explorer/Core/Core.h"

namespace Explorer
{
	/// <summary>
	/// ֡�������淶
	/// </summary>
	struct FramebufferSpecification
	{
		uint32_t Width;		//֡��������
		uint32_t Height;	//֡��������

		uint32_t Samples = 1;

		bool SwapChainTarget = false;	//�Ƿ�Ҫ��Ⱦ����Ļ
	};

	/// <summary>
	/// ֡������
	/// </summary>
	class Framebuffer
	{
	private:
		uint32_t m_RendererID;						//֡������ID
		uint32_t m_ColorAttachment;					//��ɫ������
		uint32_t m_DepthAttachment;					//��Ȼ�����
		FramebufferSpecification m_Specification;	//֡�������淶
	public:
		/// <summary>
		/// ֡������
		/// </summary>
		/// <param name="spec">֡�������淶</param>
		Framebuffer(const FramebufferSpecification& spec);
		~Framebuffer();

		/// <summary>
		/// ������С��ʹ��Ч
		/// </summary>
		void Invalidate();

		/// <summary>
		/// ��֡������
		/// </summary>
		void Bind();

		/// <summary>
		/// �����
		/// </summary>
		void Unbind();

		/// <summary>
		/// ������ɫ������ID
		/// </summary>
		/// <returns>��ɫ������ID</returns>
		uint32_t GetColorAttachmentRendererID() const { return m_ColorAttachment; }

		/// <summary>
		/// ����֡�������淶
		/// </summary>
		/// <returns>֡�������淶</returns>
		const FramebufferSpecification& GetSpecification() const { return m_Specification; }
	};
}