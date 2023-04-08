#pragma once

#include "Explorer/Components/Skybox.h"

namespace Explorer
{
	/// <summary>
	/// ��������
	/// </summary>
	class Environment
	{
	public:
		/// <summary>
		/// ��Դ����
		/// </summary>
		enum class SourceType
		{
			Skybox = 0,	//�����ӹ�ԴΪ��ɫ�Ļ�����
			Color = 1	//��պй��� �� ��ɫ���� ����Ϊ ��������ɫ
		};
	private:
		Skybox m_Skybox;													//��պ�
		bool m_SkyboxEnable = false;										//��պв����Ƿ�����
		glm::vec4 m_NoneSkyboxColor = { 0.2745f, 0.2745f, 0.2745f, 1.0f };	//��պ�ȱʧʱ������ɫ

		SourceType m_SourceType = SourceType::Color;						//������Դ����
		float m_IntensityMultiplier = 1.0f;									//��պй�ǿ�ȱ���[0, 8]:Skybox
		glm::vec3 m_AmbientColor = { 0.212f, 0.227f, 0.259f };				//��������ɫ��Color
	public:
		Environment();
		Environment(const Skybox& skybox);

		SourceType GetSourceType() const { return m_SourceType; }
		void SetSourceType(SourceType type) { m_SourceType = type; }

		bool GetSkyboxEnable() const { return m_SkyboxEnable; }
		bool& GetSkyboxEnable_Ref() { return m_SkyboxEnable; }
		void SetSkyboxEnable(bool exist) { m_SkyboxEnable = exist; }

		glm::vec4& GetNoneSkyboxColor() { return m_NoneSkyboxColor; }

		Skybox& GetSkybox() { return m_Skybox; }
		void SetSkybox(const Skybox& skybox) { m_Skybox = skybox; }

		float GetIntensityMultiplier() const { return m_IntensityMultiplier; }
		float& GetIntensityMultiplier_Ref() { return m_IntensityMultiplier; }
		void SetIntensityMultiplier(float multiplier) { m_IntensityMultiplier = multiplier; }

		glm::vec3& GetAmbientColor() { return m_AmbientColor; }
	};
}