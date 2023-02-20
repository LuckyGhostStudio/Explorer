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

		/// <summary>
		/// �ϴ�Uniform Mat4����
		/// </summary>
		/// <param name="name">uniform��shader�еı�����</param>
		/// <param name="matrix">����ֵ</param>
		void UploadUniformMat4(const std::string& name, const glm::mat4& matrix);
	};
}