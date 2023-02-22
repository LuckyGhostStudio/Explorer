#include "exppch.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Explorer
{
	Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) :m_RendererID(0)
	{
		std::string vertexSrc = ReadFile(vertexShaderPath);		//读取顶点着色器文件
		std::string fragmentSrc = ReadFile(fragmentShaderPath);	//读取片元着色器文件

		std::unordered_map<GLenum, std::string> shaderSources;	//着色器类型-源码map

		shaderSources[GL_VERTEX_SHADER] = vertexSrc;			//顶点着色器
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;		//片元着色器

		Compile(shaderSources);									//编译着色器源码
	}
	
	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);	//删除着色器程序
	}

	std::string Shader::ReadFile(const std::string& filepath)
	{
		std::string result;		//文件内容
		std::ifstream in(filepath, std::ios::in, std::ios::binary);	//输入流 二进制

		if (in) {
			in.seekg(0, std::ios::end);			//文件指针移动到文件末尾
			result.resize(in.tellg());			//重置string大小为文件大小
			in.seekg(0, std::ios::beg);			//文件指针移动到文件开头
			in.read(&result[0], result.size());	//读文件到 result 大小 size
			in.close();							//关闭文件输入流
		}
		else {
			EXP_CORE_ERROR("Could not open file '{0}'", filepath);	//无法打开文件
		}

		return result;
	}

	void Shader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		unsigned int program = glCreateProgram();	//创建程序;
		std::vector<GLenum> glShaderIDs(shaderSources.size());	//着色器ID列表

		//遍历所有类型着色器源码
		for (auto& kv : shaderSources) {
			GLenum type = kv.first;					//着色器类型
			const std::string& source = kv.second;	//着色器源码

			unsigned int shader = glCreateShader(type);		//创建type类型着色器 返回id

			const char* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);		//着色器源代码发送到GL

			glCompileShader(shader);						//编译着色器

			int isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);				//获取编译状态
			if (isCompiled == GL_FALSE)	//编译失败
			{
				int maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);			//获取编译日志信息长度

				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);	//获取编译日志信息

				glDeleteShader(shader);	//删除着色器

				EXP_CORE_ERROR("{0}", infoLog.data());							//失败信息
				EXP_CORE_ASSERT(false, "Shader Compilation Failure!");			//着色器编译失败

				break;
			}

			glAttachShader(program, shader);		//将着色器添加到程序
			glShaderIDs.push_back(shader);			//着色器ID添加到列表
		}

		glLinkProgram(program);		//链接程序

		int isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);	//获取链接状态
		if (isLinked == GL_FALSE)	//链接失败
		{
			int maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			//删除着色器
			for (auto id : glShaderIDs) {
				glDeleteShader(id);
			}

			EXP_CORE_ERROR("{0}", infoLog.data());			//失败信息
			EXP_CORE_ASSERT(false, "Shader Link Failure!");	//着色器链接失败

			return;
		}

		//分离着色器
		for (auto id : glShaderIDs) {
			glDetachShader(program, id);
		}

		m_RendererID = program;
	}
	
	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);		//使用着色器程序
	}
	
	void Shader::UnBind() const
	{
		glUseProgram(0);
	}

	void Shader::UploadUniformInt(const std::string& name, int value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1i(location, value);
	}

	void Shader::UploadUniformFloat(const std::string& name, float value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform1f(location, value);
	}

	void Shader::UploadUniformFloat2(const std::string& name, const glm::vec2& value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform2f(location, value.x, value.y);
	}

	void Shader::UploadUniformFloat3(const std::string& name, const glm::vec3& value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniform3f(location, value.x, value.y, value.z);
	}

	void Shader::UploadUniformFloat4(const std::string& name, const glm::vec4& value)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());	//获取Uniform变量位置
		glUniform4f(location, value.x, value.y, value.z, value.w);		//设置变量
	}

	void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());	//获取Uniform变量位置
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));	//设置Uniform变量（位置，变量个数，是否转置，变量地址）
	}
}