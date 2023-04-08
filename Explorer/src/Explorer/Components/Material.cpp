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
}