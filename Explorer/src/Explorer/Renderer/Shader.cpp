#include "exppch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace Explorer
{
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) :m_RendererID(0)
	{
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);		//创建顶点着色器

		const char* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);	//顶点着色器源代码发送到GL

		glCompileShader(vertexShader);	//编译顶点着色器

		int isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);	//获取编译状态
		if (isCompiled == GL_FALSE)	//编译失败
		{
			int maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);	//获取编译日志信息长度

			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);	//获取编译日志信息

			glDeleteShader(vertexShader);	//删除着色器

			EXP_CORE_ERROR("{0}", infoLog.data());							//失败信息
			EXP_CORE_ASSERT(false, "Vertex Shader Compilation Failure!");	//顶点着色器编译失败

			return;
		}

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);		//创建片元着色器

		source = fragmentSrc.c_str();
		glShaderSource(fragmentShader, 1, &source, 0);

		glCompileShader(fragmentShader);

		glGetShaderiv(fragmentShader, GL_COMPILE_STATUS, &isCompiled);
		if (isCompiled == GL_FALSE)
		{
			int maxLength = 0;
			glGetShaderiv(fragmentShader, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(fragmentShader, maxLength, &maxLength, &infoLog[0]);

			glDeleteShader(fragmentShader);
			glDeleteShader(vertexShader);

			EXP_CORE_ERROR("{0}", infoLog.data());							//失败信息
			EXP_CORE_ASSERT(false, "Fragment Shader Compilation Failure!");	//片元着色器编译失败

			return;
		}

		m_RendererID = glCreateProgram();			//创建程序
		unsigned int program = m_RendererID;

		glAttachShader(program, vertexShader);		//将顶点着色器添加到程序
		glAttachShader(program, fragmentShader);	//将片元着色器添加到程序

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

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			EXP_CORE_ERROR("{0}", infoLog.data());			//失败信息
			EXP_CORE_ASSERT(false, "Shader Link Failure!");	//着色器链接失败

			return;
		}

		//分离着色器
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}
	
	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);	//删除着色器程序
	}
	
	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);		//使用着色器程序
	}
	
	void Shader::UnBind() const
	{
		glUseProgram(0);
	}
}