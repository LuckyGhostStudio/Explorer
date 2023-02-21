#include "exppch.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Explorer
{
	Shader::Shader(const std::string& vertexSrc, const std::string& fragmentSrc) :m_RendererID(0)
	{
		unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);		//����������ɫ��

		const char* source = vertexSrc.c_str();
		glShaderSource(vertexShader, 1, &source, 0);	//������ɫ��Դ���뷢�͵�GL

		glCompileShader(vertexShader);	//���붥����ɫ��

		int isCompiled = 0;
		glGetShaderiv(vertexShader, GL_COMPILE_STATUS, &isCompiled);	//��ȡ����״̬
		if (isCompiled == GL_FALSE)	//����ʧ��
		{
			int maxLength = 0;
			glGetShaderiv(vertexShader, GL_INFO_LOG_LENGTH, &maxLength);	//��ȡ������־��Ϣ����

			std::vector<char> infoLog(maxLength);
			glGetShaderInfoLog(vertexShader, maxLength, &maxLength, &infoLog[0]);	//��ȡ������־��Ϣ

			glDeleteShader(vertexShader);	//ɾ����ɫ��

			EXP_CORE_ERROR("{0}", infoLog.data());							//ʧ����Ϣ
			EXP_CORE_ASSERT(false, "Vertex Shader Compilation Failure!");	//������ɫ������ʧ��

			return;
		}

		unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);		//����ƬԪ��ɫ��

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

			EXP_CORE_ERROR("{0}", infoLog.data());							//ʧ����Ϣ
			EXP_CORE_ASSERT(false, "Fragment Shader Compilation Failure!");	//ƬԪ��ɫ������ʧ��

			return;
		}

		m_RendererID = glCreateProgram();			//��������
		unsigned int program = m_RendererID;

		glAttachShader(program, vertexShader);		//��������ɫ����ӵ�����
		glAttachShader(program, fragmentShader);	//��ƬԪ��ɫ����ӵ�����

		glLinkProgram(program);		//���ӳ���

		int isLinked = 0;
		glGetProgramiv(program, GL_LINK_STATUS, (int*)&isLinked);	//��ȡ����״̬
		if (isLinked == GL_FALSE)	//����ʧ��
		{
			int maxLength = 0;
			glGetProgramiv(program, GL_INFO_LOG_LENGTH, &maxLength);

			std::vector<char> infoLog(maxLength);
			glGetProgramInfoLog(program, maxLength, &maxLength, &infoLog[0]);

			glDeleteProgram(program);

			glDeleteShader(vertexShader);
			glDeleteShader(fragmentShader);

			EXP_CORE_ERROR("{0}", infoLog.data());			//ʧ����Ϣ
			EXP_CORE_ASSERT(false, "Shader Link Failure!");	//��ɫ������ʧ��

			return;
		}

		//������ɫ��
		glDetachShader(program, vertexShader);
		glDetachShader(program, fragmentShader);
	}
	
	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);	//ɾ����ɫ������
	}
	
	void Shader::Bind() const
	{
		glUseProgram(m_RendererID);		//ʹ����ɫ������
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
		int location = glGetUniformLocation(m_RendererID, name.c_str());	//��ȡUniform����λ��
		glUniform4f(location, value.x, value.y, value.z, value.w);		//���ñ���
	}

	void Shader::UploadUniformMat3(const std::string& name, const glm::mat3& matrix)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());
		glUniformMatrix3fv(location, 1, GL_FALSE, glm::value_ptr(matrix));
	}

	void Shader::UploadUniformMat4(const std::string& name, const glm::mat4& matrix)
	{
		int location = glGetUniformLocation(m_RendererID, name.c_str());	//��ȡUniform����λ��
		glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(matrix));	//����Uniform������λ�ã������������Ƿ�ת�ã�������ַ��
	}
}