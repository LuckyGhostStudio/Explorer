#pragma once

namespace Explorer
{
	/// <summary>
	/// 顶点缓冲
	/// </summary>
	class VertexBuffer
	{
	private:
		uint32_t m_RendererID;		//顶点缓冲ID
	public:
		/// <summary>
		/// 顶点缓冲区
		/// </summary>
		/// <param name="vertices">顶点数据</param>
		/// <param name="size">顶点大小</param>
		/// <returns>顶点缓冲</returns>
		VertexBuffer(float* vertices, uint32_t size);
		~VertexBuffer();

		/// <summary>
		/// 绑定
		/// </summary>
		void Bind() const;

		/// <summary>
		/// 解除绑定
		/// </summary>
		void Unbind() const;
	};

	/// <summary>
	/// 索引缓冲
	/// </summary>
	class IndexBuffer
	{
	private:
		uint32_t m_RendererID;		//索引缓冲ID
		uint32_t m_Count;			//索引个数
	public:
		/// <summary>
		/// 创建索引缓冲区
		/// </summary>
		/// <param name="vertices">索引数据</param>
		/// <param name="count">索引个数</param>
		/// <returns>索引缓冲</returns>
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		/// <summary>
		/// 绑定
		/// </summary>
		void Bind() const;

		/// <summary>
		/// 解除绑定
		/// </summary>
		void Unbind() const;

		/// <summary>
		/// 返回索引个数
		/// </summary>
		/// <returns>索引个数</returns>
		uint32_t GetCount() const { return m_Count; }
	};
}