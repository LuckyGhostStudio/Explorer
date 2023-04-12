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

		shader->SetInt("u_Lights[" + std::to_string(index) + "].Enable", (int)m_Enable);		//Light����Ƿ�����

		//Light�������
		if (m_Enable) {
			shader->SetFloat3("u_Lights[" + std::to_string(index) + "].Position", position);				//��Դλ��
			shader->SetFloat3("u_Lights[" + std::to_string(index) + "].Direction", direction);				//���շ���
			shader->SetFloat3("u_Lights[" + std::to_string(index) + "].Color", m_Color);					//�ƹ���ɫ
			shader->SetFloat("u_Lights[" + std::to_string(index) + "].Intensity", m_Intensity);				//����ǿ��
			shader->SetInt("u_Lights[" + std::to_string(index) + "].RenderShadow", (int)m_CastShadow);		//�Ƿ���Ⱦ��Ӱ
			shader->SetInt("u_Lights[" + std::to_string(index) + "].Type", (int)m_Type);					//��������
			shader->SetFloat("u_Lights[" + std::to_string(index) + "].Range", m_Range);						//���հ뾶
			shader->SetFloat("u_Lights[" + std::to_string(index) + "].SpotOuterAngle", m_SpotOuterAngle);	//Spot���Ž�
			shader->SetFloat("u_Lights[" + std::to_string(index) + "].SpotInnerAngle", m_SpotInnerAngle);	//Spot���Ž�
		}
	}
}

