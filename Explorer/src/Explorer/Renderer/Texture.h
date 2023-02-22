#pragma once

namespace Explorer
{
	/// <summary>
	/// ����
	/// </summary>
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		/// <summary>
		/// ������
		/// </summary>
		/// <param name="slot">�����λ</param>
		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	/// <summary>
	/// 2D����
	/// </summary>
	class Texture2D :public Texture
	{
	private:
		uint32_t m_RendererID;	//����id
		uint32_t m_Width;		//��
		uint32_t m_Height;		//��
		std::string m_Path;		//·��
	public:
		/// <summary>
		/// 2D����
		/// </summary>
		/// <param name="path">����·��</param>
		Texture2D(const std::string& path);
		virtual ~Texture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const  override { return m_Height; }

		/// <summary>
		/// ������
		/// </summary>
		/// <param name="slot">�󶨲�λ</param>
		virtual void Bind(uint32_t slot = 0) const override;
	};
}