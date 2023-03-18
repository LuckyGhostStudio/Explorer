#include "exppch.h"
#include "Buffer.h"

#include "Renderer.h"

#include <glad/glad.h>

namespace Explorer
{
	//////////////////////////////////////////////////////////////////////////
	//////////////// Vertex Buffer ///////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	VertexBuffer::VertexBuffer(uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);								//����������
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);					//�󶨶��㻺��
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);	//�󶨶������ݣ���̬��
	}

	VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);								//����������
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);					//�󶨶��㻺��
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);	//�󶨶������ݣ���̬��
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);	//ɾ��������
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);	//�󶨶��㻺��
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	void VertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);		//�󶨶��㻺����
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);	//��������
	}

	//////////////////////////////////////////////////////////////////////////
	/////////////// Index Buffer /////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) :m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);	//����������
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);	//����������
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);	//����������
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);	//ɾ��������
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);	//����������
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}