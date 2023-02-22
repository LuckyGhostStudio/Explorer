#pragma once

namespace Explorer
{
	/// <summary>
	/// 纹理
	/// </summary>
	class Texture
	{
	public:
		virtual ~Texture() = default;

		virtual uint32_t GetWidth() const = 0;
		virtual uint32_t GetHeight() const = 0;

		/// <summary>
		/// 绑定纹理
		/// </summary>
		/// <param name="slot">纹理槽位</param>
		virtual void Bind(uint32_t slot = 0) const = 0;
	};

	/// <summary>
	/// 2D纹理
	/// </summary>
	class Texture2D :public Texture
	{
	private:
		uint32_t m_RendererID;	//纹理id
		uint32_t m_Width;		//宽
		uint32_t m_Height;		//高
		std::string m_Path;		//路径
	public:
		/// <summary>
		/// 2D纹理
		/// </summary>
		/// <param name="path">纹理路径</param>
		Texture2D(const std::string& path);
		virtual ~Texture2D();

		virtual uint32_t GetWidth() const override { return m_Width; }
		virtual uint32_t GetHeight() const  override { return m_Height; }

		/// <summary>
		/// 绑定纹理
		/// </summary>
		/// <param name="slot">绑定槽位</param>
		virtual void Bind(uint32_t slot = 0) const override;
	};
}