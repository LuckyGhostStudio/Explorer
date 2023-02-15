#pragma once

#include <string>

namespace Explorer
{
	/// <summary>
	/// 着色器
	/// </summary>
	class Shader
	{
	private:
		uint32_t m_RendererID;	//着色器ID
	public:
		/// <summary>
		/// 着色器
		/// </summary>
		/// <param name="vertexSrc">顶点着色器源代码</param>
		/// <param name="fragmentSrc">片段着色器源代码</param>
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		/// <summary>
		/// 绑定：使用着色器
		/// </summary>
		void Bind() const;

		/// <summary>
		/// 解除绑定：调试时使用
		/// </summary>
		void UnBind() const;
	};
}