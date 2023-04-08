#pragma once

#include <glm/glm.hpp>
#include "Explorer/Renderer/Texture.h"
#include "Explorer/Renderer/Shader.h"
#include "Explorer/Renderer/VertexArray.h"

namespace Explorer
{
	/// <summary>
	/// ��պ�
	/// </summary>
	class Skybox
	{
	private:
		std::shared_ptr<VertexArray> m_VertexArray;			//Cube�������� VAO
		std::shared_ptr<VertexBuffer> m_VertexBuffer;		//Cube���㻺���� VBO

		std::shared_ptr<Shader> m_Shader;					//��ɫ��

		std::shared_ptr<Cubemap> m_Cubemap;					//��������ͼ
		std::shared_ptr<Texture2D> m_PreviewTextures[6];	//Ԥ��Cube��ͼ

		glm::vec3 m_TintColor = { 0.5f, 0.5f, 0.5f };		//ɫ��

		float m_Expose = 1.0f;								//�ع��[0, 8]
		float m_Rotation = glm::radians(0.0f);				//������y����תֵ�����ȣ�
	public:
		Skybox();

		void SetCubeVertexBuffer();

		/// <summary>
		/// ����Cubemap�����һ��
		/// </summary>
		/// <param name="filepath">����·��</param>
		/// <param name="direction">��ͼ������������ͼ6������</param>
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