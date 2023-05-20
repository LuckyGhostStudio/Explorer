#include "exppch.h"
#include "VertexArray.h"

#include "Renderer.h"

#include <glad/glad.h>

namespace Explorer
{
	/// <summary>
	/// ShaderDataTypeת��ΪOpenGL��������
	/// </summary>
	/// <param name="">ShaderDataType</param>
	/// <returns>OpenGL��������</returns>
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
		glCreateVertexArrays(1, &m_RendererID);	//������������
	}

	VertexArray::~VertexArray()
	{
		glDeleteVertexArrays(1, &m_RendererID);	//ɾ����������
	}

	void VertexArray::Bind()
	{
		glBindVertexArray(m_RendererID);		//�󶨶�������
	}
	
	void VertexArray::Unbind()
	{
		glBindVertexArray(0);					//�����
	}
	
	void VertexArray::AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
	{
		EXP_CORE_ASSERT(vertexBuffer->GetLayout().GetElements().size(), "Vertex Buffer has no layout!");	//����Ԫ��Ϊ��

		glBindVertexArray(m_RendererID);	//�󶨶�������
		vertexBuffer->Bind();				//��VBO��VAO

		//���ö��㻺��������
		const auto& layout = vertexBuffer->GetLayout();			//���㻺��������

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
					glEnableVertexAttribArray(index);					//���ö�������

					glVertexAttribPointer(index,						//��������λ�ñ��
						element.GetComponentCount(),					//�����������ݸ���
						ShaderDataTypeToOpenGLBaseType(element.Type), 	//��������
						element.Normalized ? GL_TRUE : GL_FALSE, 		//�Ƿ��׼��
						layout.GetStride(), 							//�����С���ֽڣ�
						(const void*)element.Offset);					//��������ƫ�������ֽڣ�

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

		m_VertexBuffers.push_back(vertexBuffer);	//���VBO���б�
	}
	
	void VertexArray::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
	{
		glBindVertexArray(m_RendererID);	//�󶨶�������
		indexBuffer->Bind();				//��EBO��VAO

		m_IndexBuffer = indexBuffer;		//����EBO
	}
}