#include "exppch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include <glad/glad.h>

namespace Explorer
{
	/// <summary>
	/// ShaderDataType转换为OpenGL基本类型
	/// </summary>
	/// <param name="">ShaderDataType</param>
	/// <returns>OpenGL基本类型</returns>
	static GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
	{
		switch (type)
		{
			case Explorer::ShaderDataType::Float:	return GL_FLOAT;
			case Explorer::ShaderDataType::Float2:	return GL_FLOAT;
			case Explorer::ShaderDataType::Float3:	return GL_FLOAT;
			case Explorer::ShaderDataType::Float4:	return GL_FLOAT;
			case Explorer::ShaderDataType::Mat3:	return GL_FLOAT;
			case Explorer::ShaderDataType::Mat4:	return GL_FLOAT;
			case Explorer::ShaderDataType::Int:		return GL_INT;
			case Explorer::ShaderDataType::Int2:	return GL_INT;
			case Explorer::ShaderDataType::Int3:	return GL_INT;
			case Explorer::ShaderDataType::Int4:	return GL_INT;
			case Explorer::ShaderDataType::Bool:	return GL_BOOL;
		}

		EXP_CORE_ASSERT(false, "Unknow ShaderDataType!");
		return 0;
	}

	VertexArray::VertexArray()
	{
		glCreateVertexArrays(1, &m_RendererID);	//创建顶点数组
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);	//删除顶点数组
	}

	void VertexArray::Bind()
	{
		glBindVertexArray(m_RendererID);		//绑定顶点数组
	}
	
	void VertexArray::Unbind()
	{
		glBindVertexArray(0);					//解除绑定
	}
	
	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		EXP_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");	//布局元素为零

		glBindVertexArray(m_RendererID);	//绑定顶点数组
		vertexBuffer->Bind();				//绑定VBO到VAO

		//设置顶点缓冲区布局
		const auto& layout = vertexBuffer->GetLayout();			//顶点缓冲区布局

		uint32_t index = 0;
		for (const auto& element : layout)
		{
			switch (element.Type)
			{
				case ShaderDataType::Float:
				case ShaderDataType::Float2:
				case ShaderDataType::Float3:
				case ShaderDataType::Float4:
				{
					glEnableVertexAttribArray(index);					//启用顶点属性

					glVertexAttribPointer(index,						//顶点属性位置编号
						element.GetComponentCount(),					//顶点属性数据个数
						ShaderDataTypeToOpenGLBaseType(element.Type), 	//数据类型
						element.Normalized ? GL_TRUE : GL_FALSE, 		//是否标准化
						layout.GetStride(), 							//顶点大小（字节）
						(const void*)element.Offset);					//顶点属性偏移量（字节）

					index++;
					break;
				}
				case ShaderDataType::Int:
				case ShaderDataType::Int2:
				case ShaderDataType::Int3:
				case ShaderDataType::Int4:
				case ShaderDataType::Bool:
				{
					glEnableVertexAttribArray(index);
					glVertexAttribIPointer(index,
						element.GetComponentCount(),
						ShaderDataTypeToOpenGLBaseType(element.Type),
						layout.GetStride(),
						(const void*)element.Offset);

					index++;
					break;
				}
				case ShaderDataType::Mat3:
				case ShaderDataType::Mat4:
				{
					uint8_t count = element.GetComponentCount();
					for (uint8_t i = 0; i < count; i++)
					{
						glEnableVertexAttribArray(index);
						glVertexAttribPointer(index,
							count,
							ShaderDataTypeToOpenGLBaseType(element.Type),
							element.Normalized ? GL_TRUE : GL_FALSE,
							layout.GetStride(),
							(const void*)(element.Offset + sizeof(float) * count * i));
						glVertexAttribDivisor(index, 1);
						index++;
					}
					break;
				}
				default:
					EXP_CORE_ASSERT(false, "Unknown ShaderDataType!");
			}
		}

		m_VertexBuffers.push_back(vertexBuffer);	//添加VBO到列表
	}
	
	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);	//绑定顶点数组
		indexBuffer->Bind();				//绑定EBO到VAO

		m_IndexBuffer = indexBuffer;		//设置EBO
	}
}