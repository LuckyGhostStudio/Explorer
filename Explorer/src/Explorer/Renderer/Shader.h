#pragma once

#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// ��ɫ��
	/// </summary>
	class Shader
	{
	private:
		uint32_t m_RendererID;	//��ɫ��ID
		std::string m_Name;		//��ɫ������

		/// <summary>
		/// ���ļ�
		/// </summary>
		/// <param name="filepath">�ļ�·��</param>
		/// <returns>�ļ�����</returns>
		std::string ReadFile(const std::string& filepath);

		/// <summary>
		/// ������ɫ��
		/// </summary>
		/// <param name="shaderSources">��ɫ������-��ɫ��Դ��map</param>
		void Compile(std::unordered_map<unsigned int, std::string>& shaderSources);
	public:
		/// <summary>
		/// ��ɫ��
		/// </summary>
		/// <param name="filepath">��ɫ���ļ�·��(������׺��)</param>
		Shader(const std::string& filepath);
		~Shader();

		/// <summary>
		/// �󶨣�ʹ����ɫ��
		/// </summary>
		void Bind() const;

		/// <summary>
		/// ����󶨣�����ʱʹ��
		/// </summary>
		void UnBind() const;

		/// <summary>
		/// ����uniform Int����
		/// </summary>
		/// <param name="name">������</param>
		/// <param name="value">����ֵ</param>
		void SetInt(const std::string& name, int value);

		/// <summary>
		/// ����uniform Int Array����
		/// </summary>
		/// <param name="name">������</param>
		/// <param name="values">����ֵ</param>
		/// <param name="count">��������</param>
		void SetIntArray(const std::string& name, int* values, uint32_t count);

		/// <summary>
		/// ����uniform Float����
		/// </summary>
		/// <param name="name">������</param>
		/// <param name="value">����ֵ</param>
		void SetFloat(const std::string& name, float value);

		/// <summary>
		/// ����uniform Float2����
		/// </summary>
		/// <param name="name">������</param>
		/// <param name="value">����ֵ</param>
		void SetFloat2(const std::string& name, const glm::vec2& value);

		/// <summary>
		/// ����uniform Float3����
		/// </summary>
		/// <param name="name">������</param>
		/// <param name="value">����ֵ</param>
		void SetFloat3(const std::string& name, const glm::vec3& value);

		/// <summary>
		/// ����uniform Float4����
		/// </summary>
		/// <param name="name">������</param>
		/// <param name="value">����ֵ</param>
		void SetFloat4(const std::string& name, const glm::vec4& value);

		/// <summary>
		/// ����uniform Matrix4����
		/// </summary>
		/// <param name="name">������</param>
		/// <param name="value">����ֵ</param>
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
		/// �ϴ�Uniform Mat3����
		/// </summary>
		/// <param name="name">Mat3��shader�еı�����</param>
		/// <param name="matrix">Mat3</param>
		void UploadUniformMat3(const std::string& name, const glm::mat3& matrix);
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	};

	/// <summary>
	/// ��ɫ����
	/// </summary>
	class ShaderLibrary
	{
	private:
		static std::unordered_map<std::string, std::shared_ptr<Shader>> m_Shaders;		//��ɫ��map����ɫ����-��ɫ��
	public:
		static std::unordered_map<std::string, std::shared_ptr<Shader>>& GetShaders() { return m_Shaders; }
		static uint32_t GetSize() { return m_Shaders.size(); }

		/// <summary>
		/// �����ɫ��
		/// </summary>
		/// <param name="name">��ɫ����</param>
		/// <param name="shader">��ɫ��</param>
		static void Add(const std::string& name, const std::shared_ptr<Shader>& shader);

		/// <summary>
		/// �����ɫ��
		/// </summary>
		/// <param name="shader">��ɫ��</param>
		static void Add(const std::shared_ptr<Shader>& shader);

		/// <summary>
		/// ������ɫ��
		/// </summary>
		/// <param name="filepath">�ļ�·��</param>
		/// <returns>��ɫ��</returns>
		static std::shared_ptr<Shader> Load(const std::string& filepath);

		/// <summary>
		/// ������ɫ��
		/// </summary>
		/// <param name="name">��ɫ������</param>
		/// <param name="filepath">�ļ�·��</param>
		/// <returns>��ɫ��</returns>
		static std::shared_ptr<Shader> Load(const std::string& name, const std::string& filepath);

		/// <summary>
		/// ������ɫ��
		/// </summary>
		/// <param name="name">��ɫ������</param>
		/// <returns>��ɫ��</returns>
		static std::shared_ptr<Shader> Get(const std::string& name);

		/// <summary>
		/// ��ɫ���Ƿ����
		/// </summary>
		/// <param name="name">��ɫ����</param>
		/// <returns>�Ƿ����</returns>
		static bool Exists(const std::string& name);
	};
}