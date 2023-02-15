#pragma once

namespace Explorer
{
	/// <summary>
	/// ���㻺��
	/// </summary>
	class VertexBuffer
	{
	private:
		uint32_t m_RendererID;		//���㻺��ID
	public:
		/// <summary>
		/// ���㻺����
		/// </summary>
		/// <param name="vertices">��������</param>
		/// <param name="size">�����С</param>
		/// <returns>���㻺��</returns>
		VertexBuffer(float* vertices, uint32_t size);
		~VertexBuffer();

		/// <summary>
		/// ��
		/// </summary>
		void Bind() const;

		/// <summary>
		/// �����
		/// </summary>
		void Unbind() const;
	};

	/// <summary>
	/// ��������
	/// </summary>
	class IndexBuffer
	{
	private:
		uint32_t m_RendererID;		//��������ID
		uint32_t m_Count;			//��������
	public:
		/// <summary>
		/// ��������������
		/// </summary>
		/// <param name="vertices">��������</param>
		/// <param name="count">��������</param>
		/// <returns>��������</returns>
		IndexBuffer(uint32_t* indices, uint32_t count);
		~IndexBuffer();

		/// <summary>
		/// ��
		/// </summary>
		void Bind() const;

		/// <summary>
		/// �����
		/// </summary>
		void Unbind() const;

		/// <summary>
		/// ������������
		/// </summary>
		/// <returns>��������</returns>
		uint32_t GetCount() const { return m_Count; }
	};
}