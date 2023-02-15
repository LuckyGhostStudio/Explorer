#pragma once

#include <string>

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
	};
}