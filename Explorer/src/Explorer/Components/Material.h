#pragma once

#include "Explorer/Renderer/Shader.h"
#include "Explorer/Renderer/Texture.h"

namespace Explorer
{
	/// <summary>
	/// �������
	/// </summary>
	class Material
	{
	private:
		std::shared_ptr<Shader> m_Shader;				//��ɫ��

		std::shared_ptr<Texture2D> m_AlbedoTexture;		//��������ͼ����������ͼ��������ԭ����ɫ
		std::shared_ptr<Texture2D> m_SpecularTexture;	//���淴����ͼ
		float m_Shininess = 32.0f;						//����ȣ�Ӱ�쾵��߹��ɢ��/�뾶

		//Ambient �����⣺TODO:��ӵ�Environment ����
		//Normal ������ͼ
	public:

	};
}