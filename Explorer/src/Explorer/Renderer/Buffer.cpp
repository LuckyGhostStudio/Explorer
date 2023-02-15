#include "exppch.h"
#include "Buffer.h"

#include "Renderer.h"

#include <glad/glad.h>

namespace Explorer
{
	//////////////////////////////////////////////////////////////////////////
	//////////////// Vertex Buffer ///////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);	//创建缓冲区
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);	//绑定顶点缓冲
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);	//绑定顶点数据
	}

	VertexBuffer::~VertexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);	//删除缓冲区
	}

	void VertexBuffer::Bind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);	//绑定顶点缓冲
	}

	void VertexBuffer::Unbind() const
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	//////////////////////////////////////////////////////////////////////////
	/////////////// Index Buffer /////////////////////////////////////////////
	//////////////////////////////////////////////////////////////////////////

	IndexBuffer::IndexBuffer(uint32_t* indices, uint32_t count) :m_Count(count)
	{
		glCreateBuffers(1, &m_RendererID);	//创建缓冲区
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);	//绑定索引缓冲
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), indices, GL_STATIC_DRAW);	//绑定索引数据
	}

	IndexBuffer::~IndexBuffer()
	{
		glDeleteBuffers(1, &m_RendererID);	//删除缓冲区
	}

	void IndexBuffer::Bind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);	//绑定索引缓冲
	}

	void IndexBuffer::Unbind() const
	{
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
	}
}