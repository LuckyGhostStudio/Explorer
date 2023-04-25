#pragma once

#include "Explorer/Renderer/VertexArray.h"
#include <glm/glm.hpp>

namespace Explorer
{
	struct CircleVertex
	{
		glm::vec3 WorldPosition;
		glm::vec3 LocalPosition;
		glm::vec4 Color;
		float Thickness;	//���
		float Fade;			//ģ����

		int ObjectID;
	};

	/// <summary>
	/// Բ��
	/// </summary>
	class Circle
	{
	private:
		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };
		float m_Thickness = 1.0f;	//���[0, 1] �����->��ȫ���
		float m_Fade = 0.005f;

		CircleVertex m_Vertices[4];
		uint32_t m_VertexIndices[6];

		std::vector<CircleVertex> m_VertexBufferData;	//���㻺�������ݣ�����Ҫ��Ⱦ�Ķ�������

		std::shared_ptr<VertexArray> m_VertexArray;		//�������� VAO
		std::shared_ptr<VertexBuffer> m_VertexBuffer;	//���㻺���� VBO
	public:
		Circle();
		Circle(const Circle&) = default;
		Circle(float radius);
		~Circle();

		glm::vec4& GetColor() { return m_Color; }
		void SetColor(const glm::vec4& color) { m_Color = color; }

		float GetThickness() const { return m_Thickness; }
		float& GetThickness_Ref() { return m_Thickness; }
		void SetThickness(float thickness) { m_Thickness = thickness; }

		float GetFade() const { return m_Fade; }
		float& GetFade_Ref() { return m_Fade; }
		void SetFade(float fade) { m_Fade = fade; }

		CircleVertex* GetVertices() { return m_Vertices; }
		void SetVertices(const CircleVertex* vertices) { for (int i = 0; i < 4; i++) m_Vertices[i] = vertices[i]; }

		uint32_t* GetVertexIndices() { return m_VertexIndices; }
		void SetVertices(const uint32_t* indices) { for (int i = 0; i < 6; i++) m_VertexIndices[i] = indices[i]; }

		std::vector<CircleVertex>& GetVertexBufferData() { return m_VertexBufferData; }

		const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
	};
}