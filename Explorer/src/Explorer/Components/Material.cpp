#include <exppch.h>
#include "Material.h"

namespace Explorer
{
    Material::Material()
    {
        m_Shader = ShaderLibrary::Get("Standard");  //标准着色器
        m_Shader->Bind();

        m_AlbedoTexture = std::make_shared<Texture2D>(1, 1);	//创建宽高为1的纹理：纹理缺失时的预览纹理
        m_SpecularTexture = std::make_shared<Texture2D>(1, 1);	//创建宽高为1的纹理：纹理缺失时的预览纹理
        
        uint32_t loseTextureData = 0x0eeeeeff;  //颜色
        
        m_AlbedoTexture->SetData(&loseTextureData, sizeof(uint32_t));	//设置纹理数据size = 1 * 1 * 4 == sizeof(uint32_t)
        m_SpecularTexture->SetData(&loseTextureData, sizeof(uint32_t));	//设置纹理数据size = 1 * 1 * 4 == sizeof(uint32_t)

        m_AlbedoTextureExist = false;
        m_SpecularTextureExist = false;
    }

    void Material::SetAlbedoTexture(const std::string& filepath)
    {
        m_AlbedoTexture = std::make_shared<Texture2D>(filepath);    //创建反光率贴图
        m_AlbedoTexture->Bind((int)TextureType::Albedo);            //绑定反光率贴图
        m_AlbedoTextureExist = true;
    }

    void Material::SetSpecularTexture(const std::string& filepath)
    {
        m_SpecularTexture = std::make_shared<Texture2D>(filepath);  //创建高光贴图
        m_SpecularTexture->Bind((int)TextureType::Specular);        //绑定高光贴图
        m_SpecularTextureExist = true;
    }

    void Material::BindTexture()
    {
        m_AlbedoTexture->Bind((int)TextureType::Albedo);            //绑定反光率贴图
        m_SpecularTexture->Bind((int)TextureType::Specular);        //绑定高光贴图
    }
    
    void Material::SetShaderData()
    {
        m_Shader->Bind();   //绑定着色器

        m_Shader->SetInt("u_MaterialExist", m_Enable);		//材质是否存在

        //Material组件已启用
        if (m_Enable) {
            BindTexture();	//绑定纹理

            m_Shader->SetFloat4("u_Material.Color", m_Color);								        //颜色
            m_Shader->SetInt("u_Material.AlbedoTextureExist", m_AlbedoTextureExist);		        //反照率贴图是否存在
            m_Shader->SetInt("u_Material.SpecularTextureExist", m_SpecularTextureExist);	        //高光贴图是否存在
            m_Shader->SetInt("u_Material.AlbedoTexture", (int)Material::TextureType::Albedo);		//反照率贴图
            m_Shader->SetInt("u_Material.SpecularTexture", (int)Material::TextureType::Specular);	//高光贴图
            m_Shader->SetFloat("u_Material.Shininess", m_Shininess);						        //反光度
            m_Shader->SetFloat2("u_Material.Tiling", m_Tiling);							            //纹理平铺因子
            m_Shader->SetFloat2("u_Material.Offset", m_Offset);							            //纹理偏移量
        }
    }
}