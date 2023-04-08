#pragma once

#include "Explorer/Components/Skybox.h"

namespace Explorer
{
	/// <summary>
	/// 场景环境
	/// </summary>
	class Environment
	{
	public:
		/// <summary>
		/// 光源类型
		/// </summary>
		enum class SourceType
		{
			Skybox = 0,	//不叠加光源为颜色的环境光
			Color = 1	//天空盒光照 和 颜色光照 叠加为 环境光颜色
		};
	private:
		Skybox m_Skybox;													//天空盒
		bool m_SkyboxEnable = false;										//天空盒材质是否启用
		glm::vec4 m_NoneSkyboxColor = { 0.2745f, 0.2745f, 0.2745f, 1.0f };	//天空盒缺失时背景颜色

		SourceType m_SourceType = SourceType::Color;						//环境光源类型
		float m_IntensityMultiplier = 1.0f;									//天空盒光强度倍数[0, 8]:Skybox
		glm::vec3 m_AmbientColor = { 0.212f, 0.227f, 0.259f };				//环境光颜色：Color
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