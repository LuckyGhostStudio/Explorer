#pragma once

typedef unsigned int GLenum;

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
		virtual uint32_t GetRendererID() const = 0;

		/// <summary>
		/// ������������
		/// </summary>
		/// <param name="data">���ݣ�������ɫֵ��</param>
		/// <param name="size">�����С���ֽڣ�</param>
		virtual void SetData(void* data, uint32_t size) = 0;

		/// <summary>
		/// ������
		/// </summary>
		/// <param name="slot">�����λ</param>
		virtual void Bind(uint32_t slot = 0) const = 0;

		/// <summary>
		/// �Ƚ����������Ƿ����
		/// </summary>
		/// <param name="other">��һ������</param>
		/// <returns>�ȽϽ��</returns>
		virtual bool operator==(const Texture& other) const = 0;
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

		GLenum m_InternalFormat;	//�ڲ���ʽ���洢��ʽ��
		GLenum m_DataFormat;		//���ݸ�ʽ��������ʽ��
	public:
		/// <summary>
		/// 2D����
		/// </summary>
		/// <param name="width">��</param>
		/// <param name="height">��</param>
		Texture2D(uint32_t width, uint32_t height);

		/// <summary>
		/// 2D����
		/// </summary>
		/// <param name="path">����·��</param>
		Texture2D(const std::string& path);
		virtual ~Texture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const  override { return m_Height; }
		virtual uint32_t GetRendererID() const  override { return m_RendererID; }
		std::string& GetPath() { return m_Path; }

		/// <summary>
		/// ������������
		/// </summary>
		/// <param name="data">���ݣ�������ɫֵ��</param>
		/// <param name="size">�����С���ֽڣ�</param>
		virtual void SetData(void* data, uint32_t size) override;

		/// <summary>
		/// ������
		/// </summary>
		/// <param name="slot">�󶨲�λ</param>
		virtual void Bind(uint32_t slot = 0) const override;

		/// <summary>
		/// �Ƚ����������Ƿ����
		/// </summary>
		/// <param name="other">��һ������</param>
		/// <returns>�ȽϽ��</returns>
		virtual bool operator==(const Texture& other) const override
		{
			return m_RendererID == ((Texture2D&)other).m_RendererID;	//����ID���
		}
	};
}