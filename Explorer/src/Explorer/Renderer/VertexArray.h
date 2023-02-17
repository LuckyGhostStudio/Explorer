#pragma once

#include <memory>
#include "Explorer/Renderer/Buffer.h"

namespace Explorer
{
	/// <summary>
	/// ��������
	/// </summary>
	class VertexArray
	{
	private:
		uint32_t m_RendererID;										//��������id
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;	//���ڶ��������VertexBuffer�б�
		std::shared_ptr<IndexBuffer> m_IndexBuffer;					//���ڶ��������IndexBuffer
	public:
		VertexArray();
		~VertexArray();

		/// <summary>
		/// ��
		/// </summary>
		void Bind();

		/// <summary>
		/// �����
		/// </summary>
		void Unbind();

		/// <summary>
		/// ��Ӷ��㻺��
		/// </summary>
		/// <param name="vertexBuffer">���㻺��</param>
		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);

		/// <summary>
		/// ���ö�����������
		/// </summary>
		/// <param name="indexBuffer">��������</param>
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		/// <summary>
		/// ����VBO�б�
		/// </summary>
		/// <returns>���㻺���б�</returns>
		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }

		/// <summary>
		/// ����EBO
		/// </summary>
		/// <returns>��������</returns>
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	};
}