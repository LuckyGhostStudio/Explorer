#pragma once

namespace Explorer
{
	/// <summary>
	/// 着色器中的数据类型
	/// </summary>
	enum class ShaderDataType
	{
		None = 0, Float, Float2, Float3, Float4, Mat3, Mat4, Int, Int2, Int3, Int4, Bool
	};

	/// <summary>
	/// 计算着色器数据类型大小（字节）
	/// </summary>
	/// <param name="type">数据类型</param>
	/// <returns>大小（字节）</returns>
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
	/// 顶点属性元素
	/// </summary>
	struct BufferElement
	{
		std::string Name;		//名字
		ShaderDataType Type;	//数据类型
		uint32_t Size;			//大小（字节）= ShaderDataTypeSize(Type)
		uint32_t Offset;		//在顶点中的偏移量（字节）
		bool Normalized;		//是否标准化

		BufferElement() {}

		/// <summary>
		/// 顶点属性元素
		/// </summary>
		/// <param name="type">类型</param>
		/// <param name="name">名字</param>
		/// <param name="normalized">是否标准化</param>
		BufferElement(ShaderDataType type, const std::string& name, bool normalized = false)
			: Name(name), Type(type), Size(ShaderDataTypeSize(type)), Offset(0), Normalized(normalized)
		{

		}

		/// <summary>
		/// 返回组成一个顶点属性的数据个数
		/// </summary>
		/// <returns>数据个数</returns>
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
	/// 顶点缓冲布局：顶点中顶点属性的布局
	/// </summary>
	class BufferLayout
	{
	private:
		std::vector<BufferElement> m_Elements;	//顶点属性元素列表
		uint32_t m_Stride = 0;					//步幅：顶点大小（所有顶点属性Size和）

		/// <summary>
		/// 计算顶点属性偏移量和顶点大小
		/// </summary>
		void CalculateOffsetsAndStride()
		{
			uint32_t offset = 0;
			m_Stride = 0;
			//遍历所有顶点属性
			for (auto& element : m_Elements) {
				element.Offset = offset;
				offset += element.Size;		//下一个顶点属性偏移量 = sum(当前顶点属性前面所有顶点属性Size)
				m_Stride += element.Size;	//顶点大小 = sum(所有顶点Size)
			}
		}
	public:
		BufferLayout() {}

		/// <summary>
		/// 顶点缓冲布局
		/// </summary>
		/// <param name="elements">顶点属性元素列表</param>
		BufferLayout(const std::initializer_list<BufferElement>& elements) : m_Elements(elements)
		{
			CalculateOffsetsAndStride();	//计算顶点属性偏移量和顶点大小
		}

		/// <summary>
		/// 返回步幅：顶点大小
		/// </summary>
		/// <returns>顶点大小</returns>
		inline uint32_t GetStride() const { return m_Stride; }

		/// <summary>
		/// 返回顶点属性元素列表
		/// </summary>
		/// <returns></returns>
		inline const std::vector<BufferElement>& GetElements() const { return m_Elements; }

		std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
		std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
		std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
		std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }
	};

	/// <summary>
	/// 顶点缓冲
	/// </summary>
	class VertexBuffer
	{
	private:
		uint32_t m_RendererID;		//顶点缓冲ID
		BufferLayout m_Layout;		//顶点缓冲区布局
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

		/// <summary>
		/// 返回顶点缓冲区布局
		/// </summary>
		/// <returns>顶点缓冲区布局</returns>
		const BufferLayout& GetLayout() { return m_Layout; }

		/// <summary>
		/// 设置顶点缓冲布局
		/// </summary>
		/// <param name="layout">顶点缓存布局</param>
		void SetLayout(const BufferLayout& layout) { m_Layout = layout; }
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