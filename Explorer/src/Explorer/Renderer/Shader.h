#pragma once

#include <string>
#include <glm/glm.hpp>

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

		/// <summary>
		/// 上传Uniform Mat4变量
		/// </summary>
		/// <param name="name">uniform在shader中的变量名</param>
		/// <param name="matrix">变量值</param>
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	};
}