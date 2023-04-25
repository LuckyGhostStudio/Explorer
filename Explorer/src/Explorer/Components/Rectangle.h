#pragma once

#include "Line.h"

namespace Explorer
{
	/// <summary>
	/// ������
	/// </summary>
	class Rectangle
	{
	private:
		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };	//��ɫ
		LineVertex m_Vertices[4];

		std::vector<LineVertex> m_VertexBufferData;			//���㻺�������ݣ�����Ҫ��Ⱦ�Ķ�������

		std::shared_ptr<VertexArray> m_VertexArray;		//�������� VAO
		std::shared_ptr<VertexBuffer> m_VertexBuffer;	//���㻺���� VBO
	public:
		Rectangle();
		Rectangle(const Rectangle&) = default;
		~Rectangle();

		glm::vec4& GetColor() { return m_Color; }
		void SetColor(const glm::vec4& color) { m_Color = color; }

		LineVertex* GetVertices() { return m_Vertices; }
		void SetVertices(const LineVertex* vertices) { for (int i = 0; i < 4; i++) m_Vertices[i] = vertices[i]; }

		std::vector<LineVertex>& GetVertexBufferData() { return m_VertexBufferData; }

		const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
	};
}