#pragma once

namespace Explorer
{
	/// <summary>
	/// ��ɫ���е���������
	/// </summary>
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	/// <summary>
	/// ������ɫ���������ʹ�С���ֽڣ�
	/// </summary>
	/// <param name="type">��������</param>
	/// <returns>��С���ֽڣ�</returns>
	static uint32_t ShaderDataTypeSize(ShaderDataType type)
	{
		switch (type)
		{
		case ShaderDataType::Float:		return 4;
		case ShaderDataType::Float2:	return 4 * 2;
		case ShaderDataType::Float3:	return 4 * 3;
		case ShaderDataType::Float4:	return 4 * 4;
		case ShaderDataType::Mat3:		return 4 * 3 * 3;
		case ShaderDataType::Mat4:		return 4 * 4 * 4;
		case ShaderDataType::Int:		return 4;
		case ShaderDataType::Int2:		return 4 * 2;
		case ShaderDataType::Int3:		return 4 * 3;
		case ShaderDataType::Int4:		return 4 * 4;
		case ShaderDataType::Bool:		return 1;
		}

		EXP_CORE_ASSERT(false, "Unknow ShaderDataType!");
		return 0;
	}

	/// <summary>
	/// ��������Ԫ��
	/// </summary>
	struct BufferElement
	{
		std::string Name;		//����
		ShaderDataType Type;	//��������
		uint32_t Size;			//��С���ֽڣ�= ShaderDataTypeSize(Type)
		uint32_t Offset;		//�ڶ����е�ƫ�������ֽڣ�
		bool Normalized;		//�Ƿ��׼��

		BufferElement() {}

		/// <summary>
		/// ��������Ԫ��
		/// </summary>
		/// <param name="type">����</param>
		/// <param name="name">����</param>
		/// <param name="normalized">�Ƿ��׼��</param>
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{

		}

		/// <summary>
		/// �������һ���������Ե����ݸ���
		/// </summary>
		/// <returns>���ݸ���</returns>
		uint32_t GetComponentCount() const
		{
			switch (Type)
			{
			case ShaderDataType::Float:		return 1;
			case ShaderDataType::Float2:	return 2;
			case ShaderDataType::Float3:	return 3;
			case ShaderDataType::Float4:	return 4;
			case ShaderDataType::Mat3:		return 3 * 3;
			case ShaderDataType::Mat4:		return 4 * 4;
			case ShaderDataType::Int:		return 1;
			case ShaderDataType::Int2:		return 2;
			case ShaderDataType::Int3:		return 3;
			case ShaderDataType::Int4:		return 4;
			case ShaderDataType::Bool:		return 1;
			}

			EXP_CORE_ASSERT(false, "Unknow ShaderDataType!");
			return 0;
		}
	};

	/// <summary>
	/// ���㻺�岼�֣������ж������ԵĲ���
	/// </summary>
	class BufferLayout
	{
	private:
		std::vector<BufferElement> m_Elements;	//��������Ԫ���б�
		uint32_t m_Stride = 0;					//�����������С�����ж�������Size�ͣ�

		/// <summary>
		/// ���㶥������ƫ�����Ͷ����С
		/// </summary>
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			//�������ж�������
			for (auto& element : m_Elements) {
				element.Offset = offset;
				offset += element.Size;		//��һ����������ƫ���� = sum(��ǰ��������ǰ�����ж�������Size)
				m_Stride += element.Size;	//�����С = sum(���ж���Size)
			}
		}
	public:
		BufferLayout() {}

		/// <summary>
		/// ���㻺�岼��
		/// </summary>
		/// <param name="elements">��������Ԫ���б�</param>
		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements)
		{
			CalculateOffsetsAndStride();	//���㶥������ƫ�����Ͷ����С
		}

		/// <summary>
		/// ���ز����������С
		/// </summary>
		/// <returns>�����С</returns>
		inline uint32_t GetStride() const { return m_Stride; }

		/// <summary>
		/// ���ض�������Ԫ���б�
		/// </summary>
		/// <returns></returns>
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	};

	/// <summary>
	/// ���㻺��
	/// </summary>
	class VertexBuffer
	{
	private:
		uint32_t m_RendererID;		//���㻺��ID
		BufferLayout m_Layout;		//���㻺��������
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

		/// <summary>
		/// ���ض��㻺��������
		/// </summary>
		/// <returns>���㻺��������</returns>
		const BufferLayout& GetLayout() { return m_Layout; }

		/// <summary>
		/// ���ö��㻺�岼��
		/// </summary>
		/// <param name="layout">���㻺�沼��</param>
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }
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