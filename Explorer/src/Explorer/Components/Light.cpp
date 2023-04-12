#include "exppch.h"
#include "Light.h"

namespace Explorer
{
	void Light::SetIcon()
	{
		switch (m_Type)
		{
		case Type::Directional:
			m_Icon = std::make_shared<Texture2D>("assets/textures/defaults/Icons/Components/DirectionalLight_Icon.png");
			break;
		case Type::Point:
			m_Icon = std::make_shared<Texture2D>("assets/textures/defaults/Icons/Components/PointLight_Icon.png");
			break;
		case Type::Spot:
			m_Icon = std::make_shared<Texture2D>("assets/textures/defaults/Icons/Components/SpotLight_Icon.png");
			break;
		}
	}

	void Light::SetShaderData(const glm::vec3& position, const glm::vec3& direction, const std::shared_ptr<Shader>& shader, uint32_t index)
	{
		shader->Bind();

		shader->SetInt("u_Lights[" + std::to_string(index) + "].Enable", (int)m_Enable);		//Light组件是否启用

		//Light组件启用
		if (m_Enable) {
			shader->SetFloat3("u_Lights[" + std::to_string(index) + "].Position", position);				//光源位置
			shader->SetFloat3("u_Lights[" + std::to_string(index) + "].Direction", direction);				//光照方向
			shader->SetFloat3("u_Lights[" + std::to_string(index) + "].Color", m_Color);					//灯光颜色
			shader->SetFloat("u_Lights[" + std::to_string(index) + "].Intensity", m_Intensity);				//光照强度
			shader->SetInt("u_Lights[" + std::to_string(index) + "].RenderShadow", (int)m_CastShadow);		//是否渲染阴影
			shader->SetInt("u_Lights[" + std::to_string(index) + "].Type", (int)m_Type);					//光照类型
			shader->SetFloat("u_Lights[" + std::to_string(index) + "].Range", m_Range);						//光照半径
			shader->SetFloat("u_Lights[" + std::to_string(index) + "].SpotOuterAngle", m_SpotOuterAngle);	//Spot外张角
			shader->SetFloat("u_Lights[" + std::to_string(index) + "].SpotInnerAngle", m_SpotInnerAngle);	//Spot内张角
		}
	}
}

