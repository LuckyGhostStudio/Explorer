#pragma once

#include "Explorer/Core/Core.h"

namespace Explorer
{
	/// <summary>
	/// ֡�����������ʽ
	/// </summary>
	enum class FramebufferTextureFormat
	{
		None = 0,
		RGBA8,				//��ɫ
		DEFPTH24STENCIL8,	//���ģ��

		Depth = DEFPTH24STENCIL8	//Ĭ��ֵ
	};

	/// <summary>
	/// ֡����������淶
	/// </summary>
	struct FramebufferTextureSpecification
	{
		FramebufferTextureFormat TextureFormat = FramebufferTextureFormat::None;	//�����ʽ

		FramebufferTextureSpecification() = default;
		FramebufferTextureSpecification(FramebufferTextureFormat format) :TextureFormat(format) {}
	};

	/// <summary>
	/// ֡�����������淶
	/// </summary>
	struct FramebufferAttachmentSpecification
	{
		std::vector<FramebufferTextureSpecification> Attachments;	//�����б�

		FramebufferAttachmentSpecification() = default;
		FramebufferAttachmentSpecification(std::initializer_list<FramebufferTextureSpecification> attachments)
			:Attachments(attachments) {}
	};

	/// <summary>
	/// ֡�������淶
	/// </summary>
	struct FramebufferSpecification
	{
		uint32_t Width;		//֡��������
		uint32_t Height;	//֡��������
		FramebufferAttachmentSpecification Attachments;	//֡���������и���
		uint32_t Samples = 1;

		bool SwapChainTarget = false;	//�Ƿ�Ҫ��Ⱦ����Ļ
	};

	/// <summary>
	/// ֡������
	/// </summary>
	class Framebuffer
	{
	private:
		uint32_t m_RendererID = 0;					//֡������ID
		FramebufferSpecification m_Specification;	//֡�������淶
		std::vector<FramebufferTextureSpecification> m_ColorAttachmentSpecs;					//��ɫ�������淶�б�
		FramebufferTextureSpecification m_DepthAttachmentSpec = FramebufferTextureFormat::None;	//��Ȼ������淶

		std::vector<uint32_t> m_ColorAttachments;	//��ɫ������ID�б�
		uint32_t m_DepthAttachment = 0;				//��Ȼ�����ID
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
		/// ����֡��������С
		/// </summary>
		/// <param name="width">��</param>
		/// <param name="height">��</param>
		void Resize(uint32_t width, uint32_t height);

		/// <summary>
		/// ������ɫ������ID
		/// </summary>
		/// <param name="index">id�б�����</param>
		/// <returns>��ɫ������ID</returns>
		uint32_t GetColorAttachmentRendererID(uint32_t index = 0) const
		{
			EXP_CORE_ASSERT(index < m_ColorAttachments.size(), "index Խ�磡");
			return m_ColorAttachments[index];
		}

		/// <summary>
		/// ����֡�������淶
		/// </summary>
		/// <returns>֡�������淶</returns>
		const FramebufferSpecification& GetSpecification() const { return m_Specification; }
	};
}