#pragma once

#include <glm/glm.hpp>
#include "Explorer/Renderer/Texture.h"
#include "Explorer/Renderer/Shader.h"
#include "Explorer/Renderer/VertexArray.h"

namespace Explorer
{
	/// <summary>
	/// 天空盒
	/// </summary>
	class Skybox
	{
	private:
		std::shared_ptr<VertexArray> m_VertexArray;			//Cube顶点数组 VAO
		std::shared_ptr<VertexBuffer> m_VertexBuffer;		//Cube顶点缓冲区 VBO

		std::shared_ptr<Shader> m_Shader;					//着色器

		std::shared_ptr<Cubemap> m_Cubemap;					//立方体贴图
		std::shared_ptr<Texture2D> m_PreviewTextures[6];	//预览Cube贴图

		glm::vec3 m_TintColor = { 0.5f, 0.5f, 0.5f };		//色调

		float m_Expose = 1.0f;								//曝光度[0, 8]
		float m_Rotation = glm::radians(0.0f);				//立方体y轴旋转值（弧度）
	public:
		Skybox();

		void SetCubeVertexBuffer();

		/// <summary>
		/// 设置Cubemap纹理的一面
		/// </summary>
		/// <param name="filepath">纹理路径</param>
		/// <param name="direction">贴图方向：立方体贴图6个方向</param>
		void SetCubemapOneSideTexture(const std::string& filepath, Cubemap::TextureDirection direction);

		std::shared_ptr<Cubemap>& GetCubemap() { return m_Cubemap; }

		const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }

		std::shared_ptr<Texture2D>* GetPreviewTextures() { return m_PreviewTextures; }

		std::shared_ptr<Shader>& GetShader() { return m_Shader; }
		void SetShader(std::shared_ptr<Shader>& shader) { m_Shader = shader; m_Shader->Bind(); }

		glm::vec3& GetTintColor() { return m_TintColor; }
		void SetTintColor(const glm::vec3& color) { m_TintColor = color; }

		float GetExpose() const { return m_Expose; }
		float& GetExpose_Ref() { return m_Expose; }
		void SetExpose(float expose) { m_Expose = expose; }

		float GetRotation() const { return m_Rotation; }
		float& GetRotation_Ref() { return m_Rotation; }
		void SetRotation(float rotation) { m_Rotation = rotation; }
	};
}