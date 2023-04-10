#pragma once

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
		std::string m_Name;		//着色器名字

		/// <summary>
		/// 读文件
		/// </summary>
		/// <param name="filepath">文件路径</param>
		/// <returns>文件内容</returns>
		std::string ReadFile(const std::string& filepath);

		/// <summary>
		/// 编译着色器
		/// </summary>
		/// <param name="shaderSources">着色器类型-着色器源码map</param>
		void Compile(std::unordered_map<unsigned int, std::string>& shaderSources);
	public:
		/// <summary>
		/// 着色器
		/// </summary>
		/// <param name="filepath">着色器文件路径(不含后缀名)</param>
		Shader(const std::string& filepath);
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
		/// 设置uniform Int变量
		/// </summary>
		/// <param name="name">变量名</param>
		/// <param name="value">变量值</param>
		void SetInt(const std::string& name, int value);

		/// <summary>
		/// 设置uniform Int Array变量
		/// </summary>
		/// <param name="name">变量名</param>
		/// <param name="values">变量值</param>
		/// <param name="count">变量个数</param>
		void SetIntArray(const std::string& name, int* values, uint32_t count);

		/// <summary>
		/// 设置uniform Float变量
		/// </summary>
		/// <param name="name">变量名</param>
		/// <param name="value">变量值</param>
		void SetFloat(const std::string& name, float value);

		/// <summary>
		/// 设置uniform Float2变量
		/// </summary>
		/// <param name="name">变量名</param>
		/// <param name="value">变量值</param>
		void SetFloat2(const std::string& name, const glm::vec2& value);

		/// <summary>
		/// 设置uniform Float3变量
		/// </summary>
		/// <param name="name">变量名</param>
		/// <param name="value">变量值</param>
		void SetFloat3(const std::string& name, const glm::vec3& value);

		/// <summary>
		/// 设置uniform Float4变量
		/// </summary>
		/// <param name="name">变量名</param>
		/// <param name="value">变量值</param>
		void SetFloat4(const std::string& name, const glm::vec4& value);

		/// <summary>
		/// 设置uniform Matrix4变量
		/// </summary>
		/// <param name="name">变量名</param>
		/// <param name="value">变量值</param>
		void SetMat4(const std::string& name, const glm::mat4& value);

		const std::string GetName() const { return m_Name; }
		void SetName(const std::string& name) { m_Name = name; }

		void UploadUniformInt(const std::string& name, int value);
		void UploadUniformIntArray(const std::string& name, int* values, uint32_t count);
		void UploadUniformFloat(const std::string& name, float value);
		void UploadUniformFloat2(const std::string& name, const glm::vec2& value);
		void UploadUniformFloat3(const std::string& name, const glm::vec3& value);
		void UploadUniformFloat4(const std::string& name, const glm::vec4& value);

		/// <summary>
		/// 上传Uniform Mat3变量
		/// </summary>
		/// <param name="name">Mat3在shader中的变量名</param>
		/// <param name="matrix">Mat3</param>
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	};

	/// <summary>
	/// 着色器库
	/// </summary>
	class ShaderLibrary
	{
	private:
		static std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;		//着色器map：着色器名-着色器
	public:
		static std::unordered_map<std::string, std::shared_ptr<Shader>>& GetShaders() { return m_Shaders; }
		static uint32_t GetSize() { return m_Shaders.size(); }

		/// <summary>
		/// 添加着色器
		/// </summary>
		/// <param name="name">着色器名</param>
		/// <param name="shader">着色器</param>
		static void Add(const std::string& name, const std::shared_ptr<Shader>& shader);

		/// <summary>
		/// 添加着色器
		/// </summary>
		/// <param name="shader">着色器</param>
		static void Add(const std::shared_ptr<Shader>& shader);

		/// <summary>
		/// 加载着色器
		/// </summary>
		/// <param name="filepath">文件路径</param>
		/// <returns>着色器</returns>
		static std::shared_ptr<Shader> Load(const std::string& filepath);

		/// <summary>
		/// 加载着色器
		/// </summary>
		/// <param name="name">着色器名称</param>
		/// <param name="filepath">文件路径</param>
		/// <returns>着色器</returns>
		static std::shared_ptr<Shader> Load(const std::string& name, const std::string& filepath);

		/// <summary>
		/// 返回着色器
		/// </summary>
		/// <param name="name">着色器名称</param>
		/// <returns>着色器</returns>
		static std::shared_ptr<Shader> Get(const std::string& name);

		/// <summary>
		/// 着色器是否存在
		/// </summary>
		/// <param name="name">着色器名</param>
		/// <returns>是否存在</returns>
		static bool Exists(const std::string& name);
	};
}