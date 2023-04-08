#include <exppch.h>
#include "Material.h"

namespace Explorer
{
    Material::Material()
    {
        m_Shader = ShaderLibrary::Get("Standard");  //��׼��ɫ��
        m_Shader->Bind();

        m_AlbedoTexture = std::make_shared<Texture2D>(1, 1);	//�������Ϊ1����������ȱʧʱ��Ԥ������
        m_SpecularTexture = std::make_shared<Texture2D>(1, 1);	//�������Ϊ1����������ȱʧʱ��Ԥ������
        
        uint32_t loseTextureData = 0x0eeeeeff;  //��ɫ
        
        m_AlbedoTexture->SetData(&loseTextureData, sizeof(uint32_t));	//������������size = 1 * 1 * 4 == sizeof(uint32_t)
        m_SpecularTexture->SetData(&loseTextureData, sizeof(uint32_t));	//������������size = 1 * 1 * 4 == sizeof(uint32_t)

        m_AlbedoTextureExist = false;
        m_SpecularTextureExist = false;
    }

    void Material::SetAlbedoTexture(const std::string& filepath)
    {
        m_AlbedoTexture = std::make_shared<Texture2D>(filepath);    //������������ͼ
        m_AlbedoTexture->Bind((int)TextureType::Albedo);            //�󶨷�������ͼ
        m_AlbedoTextureExist = true;
    }

    void Material::SetSpecularTexture(const std::string& filepath)
    {
        m_SpecularTexture = std::make_shared<Texture2D>(filepath);  //�����߹���ͼ
        m_SpecularTexture->Bind((int)TextureType::Specular);        //�󶨸߹���ͼ
        m_SpecularTextureExist = true;
    }

    void Material::BindTexture()
    {
        m_AlbedoTexture->Bind((int)TextureType::Albedo);            //�󶨷�������ͼ
        m_SpecularTexture->Bind((int)TextureType::Specular);        //�󶨸߹���ͼ
    }
}