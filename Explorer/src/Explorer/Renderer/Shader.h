#pragma once

#include <string>
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
	public:
		/// <summary>
		/// ��ɫ��
		/// </summary>
		/// <param name="vertexSrc">������ɫ��Դ����</param>
		/// <param name="fragmentSrc">Ƭ����ɫ��Դ����</param>
		Shader(const std::string& vertexSrc, const std::string& fragmentSrc);
		~Shader();

		/// <summary>
		/// �󶨣�ʹ����ɫ��
		/// </summary>
		void Bind() const;

		/// <summary>
		/// ����󶨣�����ʱʹ��
		/// </summary>
		void UnBind() const;

		void UploadUniformInt(const std::string& name, int value);

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
}