#include "exppch.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

namespace Explorer
{
	Shader::Shader(const std::string& vertexShaderPath, const std::string& fragmentShaderPath) :m_RendererID(0)
	{
		std::string vertexSrc = ReadFile(vertexShaderPath);		//��ȡ������ɫ���ļ�
		std::string fragmentSrc = ReadFile(fragmentShaderPath);	//��ȡƬԪ��ɫ���ļ�

		std::unordered_map<GLenum, std::string> shaderSources;	//��ɫ������-Դ��map

		shaderSources[GL_VERTEX_SHADER] = vertexSrc;			//������ɫ��
		shaderSources[GL_FRAGMENT_SHADER] = fragmentSrc;		//ƬԪ��ɫ��

		Compile(shaderSources);									//������ɫ��Դ��
	}
	
	Shader::~Shader()
	{
		glDeleteProgram(m_RendererID);	//ɾ����ɫ������
	}

	std::string Shader::ReadFile(const std::string& filepath)
	{
		std::string result;		//�ļ�����
		std::ifstream in(filepath, std::ios::in, std::ios::binary);	//������ ������

		if (in) {
			in.seekg(0, std::ios::end);			//�ļ�ָ���ƶ����ļ�ĩβ
			result.resize(in.tellg());			//����string��СΪ�ļ���С
			in.seekg(0, std::ios::beg);			//�ļ�ָ���ƶ����ļ���ͷ
			in.read(&result[0], result.size());	//���ļ��� result ��С size
			in.close();							//�ر��ļ�������
		}
		else {
			EXP_CORE_ERROR("Could not open file '{0}'", filepath);	//�޷����ļ�
		}

		return result;
	}

	void Shader::Compile(std::unordered_map<GLenum, std::string>& shaderSources)
	{
		unsigned int program = glCreateProgram();	//��������;
		std::vector<GLenum> glShaderIDs(shaderSources.size());	//��ɫ��ID�б�

		//��������������ɫ��Դ��
		for (auto& kv : shaderSources) {
			GLenum type = kv.first;					//��ɫ������
			const std::string& source = kv.second;	//��ɫ��Դ��

			unsigned int shader = glCreateShader(type);		//����type������ɫ�� ����id

			const char* sourceCStr = source.c_str();
			glShaderSource(shader, 1, &sourceCStr, 0);		//��ɫ��Դ���뷢�͵�GL

			glCompileShader(shader);						//������ɫ��

			int isCompiled = 0;
			glGetShaderiv(shader, GL_COMPILE_STATUS, &isCompiled);				//��ȡ����״̬
			if (isCompiled == GL_FALSE)	//����ʧ��
			{
				int maxLength = 0;
				glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &maxLength);			//��ȡ������־��Ϣ����

				std::vector<char> infoLog(maxLength);
				glGetShaderInfoLog(shader, maxLength, &maxLength, &infoLog[0]);	//��ȡ������־��Ϣ

				glDeleteShader(shader);	//ɾ����ɫ��

				EXP_CORE_ERROR("{0}", infoLog.data());							//ʧ����Ϣ
				EXP_CORE_ASSERT(false, "Shader Compilation Failure!");			//��ɫ������ʧ��

				break;
			}

			glAttachShader(program, shader);		//����ɫ����ӵ�����
			glShaderIDs.push_back(shader);			//��ɫ��ID��ӵ��б�
		}

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

			//ɾ����ɫ��
			for (auto id : glShaderIDs) {
				glDeleteShader(id);
			}

			EXP_CORE_ERROR("{0}", infoLog.data());			//ʧ����Ϣ
			EXP_CORE_ASSERT(false, "Shader Link Failure!");	//��ɫ������ʧ��

			return;
		}

		//������ɫ��
		for (auto id : glShaderIDs) {
			glDetachShader(program, id);
		}

		m_RendererID = program;
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