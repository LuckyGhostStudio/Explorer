#pragma once

#include <memory>
#include "Explorer/Renderer/Buffer.h"

namespace Explorer
{
	/// <summary>
	/// 顶点数组
	/// </summary>
	class VertexArray
	{
	private:
		uint32_t m_RendererID;										//顶点数组id
		std::vector<std::shared_ptr<VertexBuffer>> m_VertexBuffers;	//绑定在顶点数组的VertexBuffer列表
		std::shared_ptr<IndexBuffer> m_IndexBuffer;					//绑定在顶点数组的IndexBuffer
	public:
		VertexArray();
		~VertexArray();

		/// <summary>
		/// 绑定
		/// </summary>
		void Bind();

		/// <summary>
		/// 解除绑定
		/// </summary>
		void Unbind();

		/// <summary>
		/// 添加顶点缓冲
		/// </summary>
		/// <param name="vertexBuffer">顶点缓冲</param>
		void AddVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);

		/// <summary>
		/// 设置顶点索引缓冲
		/// </summary>
		/// <param name="indexBuffer">索引缓冲</param>
		void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);

		/// <summary>
		/// 返回VBO列表
		/// </summary>
		/// <returns>顶点缓冲列表</returns>
		const std::vector<std::shared_ptr<VertexBuffer>>& GetVertexBuffers() const { return m_VertexBuffers; }

		/// <summary>
		/// 返回EBO
		/// </summary>
		/// <returns>索引缓冲</returns>
		const std::shared_ptr<IndexBuffer>& GetIndexBuffer() const { return m_IndexBuffer; }
	};
}