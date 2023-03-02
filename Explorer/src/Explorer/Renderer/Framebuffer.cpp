#include "exppch.h"
#include "Framebuffer.h"

#include <glad/glad.h>

namespace Explorer
{
	static uint32_t s_MaxFramebufferSize = 8192;	//֡����������С

	Framebuffer::Framebuffer(const FramebufferSpecification& spec) :m_Specification(spec)
	{
		Invalidate();
	}
	
	Framebuffer::~Framebuffer()
	{
		glDeleteFramebuffers(1, &m_RendererID);	//ɾ��֡������
		glDeleteTextures(1, &m_ColorAttachment);	//ɾ����ɫ������
		glDeleteTextures(1, &m_DepthAttachment);	//ɾ����Ȼ�����
	}
	
	void Framebuffer::Invalidate()
	{
		//֡����������
		if (m_RendererID) {
			glDeleteFramebuffers(1, &m_RendererID);		//ɾ��֡������
			glDeleteTextures(1, &m_ColorAttachment);	//ɾ����ɫ������
			glDeleteTextures(1, &m_DepthAttachment);	//ɾ����Ȼ�����
		}

		glCreateFramebuffers(1, &m_RendererID);				//����֡������
		glBindFramebuffer(GL_FRAMEBUFFER, m_RendererID);	//��֡������

		glCreateTextures(GL_TEXTURE_2D, 1, &m_ColorAttachment);	//������ɫ������
		glBindTexture(GL_TEXTURE_2D, m_ColorAttachment);		//����ɫ������
		//����ͼ��
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA8, m_Specification.Width, m_Specification.Height, 0, GL_RGBA, GL_UNSIGNED_BYTE, nullptr);
		//�����������
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);	//��С������ ���Բ�ֵ
		glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);	//�Ŵ������ ���Բ�ֵ

		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, m_ColorAttachment, 0);	//֡��������ɫ����

		glCreateTextures(GL_TEXTURE_2D, 1, &m_DepthAttachment);	//������Ȼ�����
		glBindTexture(GL_TEXTURE_2D, m_DepthAttachment);		//����Ȼ�����
		//��Ȼ���������洢 24λ��Ȼ����� 8λģ�建����
		glTexStorage2D(GL_TEXTURE_2D, 1, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height);
		//glTexImage2D(GL_TEXTURE_2D, 0, GL_DEPTH24_STENCIL8, m_Specification.Width, m_Specification.Height, 0, GL_DEPTH_STENCIL, GL_UNSIGNED_INT_24_8, NULL);
		glFramebufferTexture2D(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_TEXTURE_2D, m_DepthAttachment, 0);	//֡�������������

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
}