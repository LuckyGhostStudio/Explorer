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

    Material::Material(const std::string& albedoPath, const std::string& specularPath)
    {
        m_Shader = ShaderLibrary::Get("Standard");  //��׼��ɫ��
        m_Shader->Bind();

        SetAlbedoTexture(albedoPath);       //����Albedo��ͼ
        SetSpecularTexture(specularPath);   //����Specular��ͼ
    }

    Material::Material(const std::shared_ptr<Texture2D>& albedoTexture, const std::shared_ptr<Texture2D>& specularTexture)
    {
        SetAlbedoTexture(albedoTexture);       //����Albedo��ͼ
        SetSpecularTexture(specularTexture);   //����Specular��ͼ
    }

    void Material::SetAlbedoTexture(const std::string& filepath)
    {
        m_AlbedoTexture = std::make_shared<Texture2D>(filepath);    //����Albedo��ͼ
        m_AlbedoTexture->Bind((int)TextureType::Albedo);            //��Albedo��ͼ
        m_AlbedoTextureExist = true;
    }

    void Material::SetSpecularTexture(const std::string& filepath)
    {
        m_SpecularTexture = std::make_shared<Texture2D>(filepath);  //����Specular��ͼ
        m_SpecularTexture->Bind((int)TextureType::Specular);        //��Specular��ͼ
        m_SpecularTextureExist = true;
    }

    void Material::SetAlbedoTexture(const std::shared_ptr<Texture2D>& texture)
    {
        m_AlbedoTexture = texture;                          //����Albedo��ͼ
        m_AlbedoTexture->Bind((int)TextureType::Albedo);    //��Albedo��ͼ
        m_AlbedoTextureExist = true;
    }

    void Material::SetSpecularTexture(const std::shared_ptr<Texture2D>& texture)
    {
        m_SpecularTexture = texture;                                //����Specular��ͼ
        m_SpecularTexture->Bind((int)TextureType::Specular);        //��Specular��ͼ
        m_SpecularTextureExist = true;
    }

    void Material::BindTexture()
    {
        m_AlbedoTexture->Bind((int)TextureType::Albedo);            //��Albedo��ͼ
        m_SpecularTexture->Bind((int)TextureType::Specular);        //��Specular��ͼ
    }
    
    void Material::SetShaderData()
    {
        m_Shader->Bind();   //����ɫ��

        m_Shader->SetInt("u_MaterialExist", m_Enable);		//�����Ƿ����

        //Material���������
        if (m_Enable) {
            BindTexture();	//������

            m_Shader->SetFloat4("u_Material.Color", m_Color);								        //��ɫ
            m_Shader->SetInt("u_Material.AlbedoTextureExist", m_AlbedoTextureExist);		        //��������ͼ�Ƿ����
            m_Shader->SetInt("u_Material.SpecularTextureExist", m_SpecularTextureExist);	        //�߹���ͼ�Ƿ����
            m_Shader->SetInt("u_Material.AlbedoTexture", (int)Material::TextureType::Albedo);		//��������ͼ
            m_Shader->SetInt("u_Material.SpecularTexture", (int)Material::TextureType::Specular);	//�߹���ͼ
            m_Shader->SetFloat("u_Material.Shininess", m_Shininess);						        //�����
            m_Shader->SetFloat2("u_Material.Tiling", m_Tiling);							            //����ƽ������
            m_Shader->SetFloat2("u_Material.Offset", m_Offset);							            //����ƫ����
        }
    }
}