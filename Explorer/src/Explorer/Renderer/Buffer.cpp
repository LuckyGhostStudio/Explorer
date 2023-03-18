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
		glCreateBuffers(1, &m_RendererID);								//创建缓冲区
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);					//绑定顶点缓冲
		glBufferData(GL_ARRAY_BUFFER, size, nullptr, GL_DYNAMIC_DRAW);	//绑定顶点数据（动态）
	}

	VertexBuffer::VertexBuffer(float* vertices, uint32_t size)
	{
		glCreateBuffers(1, &m_RendererID);								//创建缓冲区
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);					//绑定顶点缓冲
		glBufferData(GL_ARRAY_BUFFER, size, vertices, GL_STATIC_DRAW);	//绑定顶点数据（静态）
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

	void VertexBuffer::SetData(const void* data, uint32_t size)
	{
		glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);		//绑定顶点缓冲区
		glBufferSubData(GL_ARRAY_BUFFER, 0, size, data);	//设置数据
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