#include "exppch.h"
#include "Framebuffer.h"

#include <glad/glad.h>

namespace Explorer
{
	static uint32_t s_MaxFramebufferSize = 8192;	//֡����������С

	namespace Utils
	{
		/// <summary>
		/// �������Ŀ��
		/// </summary>
		/// <param name="multisampled">�Ƿ���ز���</param>
		/// <returns>��������</returns>
		static GLenum TextureTarget(bool multisampled)
		{
			return multisampled ? GL_TEXTURE_2D_MULTISAMPLE : GL_TEXTURE_2D;	//Texture2D���ز��� Texture2D
		}

		/// <summary>
		/// ��������������
		/// </summary>
		/// <param name="multisampled">�Ƿ���ز���</param>
		/// <param name="outID">�������ID</param>
		/// <param name="count">��������</param>
		static void CreateTextures(bool multisampled, uint32_t* outID, uint32_t count)
		{
			glCreateTextures(TextureTarget(multisampled), count, outID);	//��������
		}

		/// <summary>
		/// ������
		/// </summary>
		/// <param name="multisampled">�Ƿ���ز���</param>
		/// <param name="id">����id</param>
		static void BindTexture(bool multisampled, uint32_t id)
		{
			glBindTexture(TextureTarget(multisampled), id);	//������
		}

		/// <summary>
		/// ������ɫ����
		/// </summary>
		/// <param name="id">��ɫ������id</param>
		/// <param name="samples">��������</param>
		/// <param name="internalFormat">�ڲ���ʽ</param>
		/// <param name="format">�ⲿ��ʽ</param>
		/// <param name="width">��</param>
		/// <param name="height">��</param>
		/// <param name="index">��ɫ������id����</param>
		static void AttachColorTexture(uint32_t id, int samples, GLenum internalFormat, GLenum format, uint32_t width, uint32_t height, int index)
		{
			bool multisampled = samples > 1;	//�Ƿ��Ƕ��ز���
			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, internalFormat, width, height, GL_FALSE);	//���ز�������ͼ��
			}
			else {
				//����ͼ��- - �ڲ���ʽ���洢��ʽ��- - - �ⲿ��ʽ�����ʸ�ʽ��
				glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, format, GL_UNSIGNED_BYTE, nullptr);
				//�����������
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//��С������ ���Բ�ֵ
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//�Ŵ������ ���Բ�ֵ
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			//��ɫ�����ӵ�֡������
			glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0 + index, TextureTarget(multisampled), id, 0);
		}

		static void AttachDepthTexture(uint32_t id, int samples, GLenum format, GLenum attachmentType, uint32_t width, uint32_t height)
		{
			bool multisampled = samples > 1;	//�Ƿ��Ƕ��ز���
			if (multisampled) {
				glTexImage2DMultisample(GL_TEXTURE_2D_MULTISAMPLE, samples, format, width, height, GL_FALSE);	//���ز�������ͼ��
			}
			else {
				//��Ȼ���������洢 24λ��Ȼ����� 8λģ�建����
				glTexStorage2D(GL_TEXTURE_2D, 1, format, width, height);
				//�����������
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//��С������ ���Բ�ֵ
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//�Ŵ������ ���Բ�ֵ
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_R, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
				glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
			}

			//��������ӵ�֡������
			glFramebufferTexture2D(GL_FRAMEBUFFER, attachmentType, TextureTarget(multisampled), id, 0);
		}

		/// <summary>
		/// �����������Ƿ�����ȸ�ʽ
		/// </summary>
		/// <param name="format">��ʽ</param>
		/// <returns>���</returns>
		static bool IsDepthFormat(FramebufferTextureFormat format)
		{
			switch (format)
			{
				case FramebufferTextureFormat::DEFPTH24STENCIL8: return true;
			}

			return false;
		}

		/// <summary>
		/// ֡�����������ʽתGL
		/// </summary>
		/// <param name="format">֡�����������ʽ</param>
		/// <returns>GL�����ʽ</returns>
		static GLenum FramebufferTextureFormatToGL(FramebufferTextureFormat format)
		{
			switch (format) {
			case FramebufferTextureFormat::RGBA8: return GL_RGBA8;
			case FramebufferTextureFormat::RED_INTEGER: return GL_RED_INTEGER;
			}

			EXP_CORE_ASSERT(false, "No format");
			return 0;
		}
	}

	Framebuffer::Framebuffer(const FramebufferSpecification& spec) :m_Specification(spec)
	{
		//����֡�������淶�����б�
		for (auto format : spec.Attachments.Attachments) {
			if (!Utils::IsDepthFormat(format.TextureFormat)) {	//������ȸ�ʽ
				m_ColorAttachmentSpecs.emplace_back(format);	//��ӵ���ɫ�������淶�б�
			}
			else {
				m_DepthAttachmentSpec = format;	//��Ȼ������淶��ʽ
			}
		}

		Invalidate();
	}
	
	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);	//ɾ��֡������
		glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());	//ɾ����ɫ������
		glDeleteTextures(1, &m_DepthAttachment);	//ɾ����Ȼ�����
	}
	
	void Framebuffer::Invalidate()
	{
		//֡����������
		if (m_RendererID) {
			glDeleteFramebuffers(1, &m_RendererID);		//ɾ��֡������
			glDeleteTextures(m_ColorAttachments.size(), m_ColorAttachments.data());	//ɾ����ɫ������
			glDeleteTextures(1, &m_DepthAttachment);	//ɾ����Ȼ�����

			m_ColorAttachments.clear();	//�����ɫ������id�б�
			m_DepthAttachment = 0;		//������Ȼ�����id
		}

		glCreateFramebuffers(1, &m_RendererID);				//����֡������
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);	//��֡������

		bool multisampled = m_Specification.Samples > 1;	//�Ƿ��Ƕ��ز���

		//��ɫ��������������
		if (m_ColorAttachmentSpecs.size()) {
			m_ColorAttachments.resize(m_ColorAttachmentSpecs.size());	//������ɫ������ID�б��С
			Utils::CreateTextures(multisampled, m_ColorAttachments.data(), m_ColorAttachments.size());	//������ɫ����������
			//������ɫ������ID�б�
			for (size_t i = 0; i < m_ColorAttachments.size(); i++) {
				Utils::BindTexture(multisampled, m_ColorAttachments[i]);	//����ɫ����������
				switch (m_ColorAttachmentSpecs[i].TextureFormat) {
					case FramebufferTextureFormat::RGBA8:
						Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_RGBA8, GL_RGBA, m_Specification.Width, m_Specification.Height, i);
						break;
					case FramebufferTextureFormat::RED_INTEGER:
						Utils::AttachColorTexture(m_ColorAttachments[i], m_Specification.Samples, GL_R32I, GL_RED_INTEGER, m_Specification.Width, m_Specification.Height, i);
						break;
				}
			}
		}

		//��Ȼ�������������
		if (m_DepthAttachmentSpec.TextureFormat != FramebufferTextureFormat::None) {
			Utils::CreateTextures(multisampled, &m_DepthAttachment, 1);	//������Ȼ���������
			Utils::BindTexture(multisampled, m_DepthAttachment);		//����Ȼ���������
			switch (m_DepthAttachmentSpec.TextureFormat) {
				case FramebufferTextureFormat::DEFPTH24STENCIL8:
					Utils::AttachDepthTexture(m_DepthAttachment, m_Specification.Samples, GL_DEPTH24_STENCIL8, GL_DEPTH_STENCIL_ATTACHMENT, m_Specification.Width, m_Specification.Height);
					break;
			}
		}

		if (m_ColorAttachments.size() > 1) {
			EXP_CORE_ASSERT(m_ColorAttachments.size() <= 4, "m_ColorAttachments.size() > 4");	//��ɫ������������4
			GLenum buffers[4] = { GL_COLOR_ATTACHMENT0, GL_COLOR_ATTACHMENT1, GL_COLOR_ATTACHMENT2, GL_COLOR_ATTACHMENT3 };
			glDrawBuffers(m_ColorAttachments.size(), buffers);	//���ƻ�����
		}
		else if (m_ColorAttachments.empty()) {	//��ɫ��������
			glDrawBuffer(GL_NONE);	//Depth ������
		}

		//���֡������״̬
		EXP_CORE_ASSERT(glCheckFramebufferStatus(GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE, "Framebuffer is incomplete��");

		glBindFramebuffer(GL_FRAMEBUFFER, 0);	//�����
	}
	
	void Framebuffer::Bind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);	//��֡������
		glViewport(0, 0, m_Specification.Width, m_Specification.Height);	//�����ӿڴ�С
	}
	
	void Framebuffer::Unbind()
	{
		glBindFramebuffer(GL_FRAMEBUFFER, 0);	//�����
	}
	
	void Framebuffer::Resize(uint32_t width, uint32_t height)
	{
		if (width == 0 || height == 0 || width > s_MaxFramebufferSize || height > s_MaxFramebufferSize) {
			EXP_CORE_WARN("Attempted to resize framebuffer to {0}, {1}", width, height);
			return;
		}

		m_Specification.Width = width;
		m_Specification.Height = height;

		Invalidate();
	}
	
	int Framebuffer::ReadPixel(uint32_t attachmentIndex, int x, int y)
	{
		EXP_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "indexԽ�磡");

		glReadBuffer(GL_COLOR_ATTACHMENT0 + attachmentIndex);	//����attachmentIndex����ɫ������

		int pixelData;
		glReadPixels(x, y, 1, 1, GL_RED_INTEGER, GL_INT, &pixelData);	//��x,yλ�õ����� ����int������������

		return pixelData;	//�����attachmentIndex��ɫ������������
	}

	void Framebuffer::ClearAttachment(uint32_t attachmentIndex, int value)
	{
		EXP_CORE_ASSERT(attachmentIndex < m_ColorAttachments.size(), "indexԽ�磡");

		auto& spec = m_ColorAttachmentSpecs[attachmentIndex];	//attachmentIndex����ɫ�������淶

		glClearTexImage(m_ColorAttachments[attachmentIndex], 0,
			Utils::FramebufferTextureFormatToGL(spec.TextureFormat), GL_INT, &(value));	//���attachmentIndex����ɫ��������ֵΪvalue
	}
}