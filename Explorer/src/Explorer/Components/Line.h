#pragma once

#include "Explorer/Renderer/VertexArray.h"
#include <glm/glm.hpp>

namespace Explorer
{
	struct LineVertex
	{
		glm::vec3 Position;
		glm::vec4 Color;

		int ObjectID;
	};

	/// <summary>
	/// ֱ��
	/// </summary>
	class Line
	{
	private:
		glm::vec4 m_Color{ 1.0f, 1.0f, 1.0f, 1.0f };	//��ɫ
		LineVertex m_Vertices[2];						//��� �յ�
		float m_Width = 2.0f;							//�߿�

		std::vector<LineVertex> m_VertexBufferData;		//���㻺�������ݣ�����Ҫ��Ⱦ�Ķ�������

		std::shared_ptr<VertexArray> m_VertexArray;		//�������� VAO
		std::shared_ptr<VertexBuffer> m_VertexBuffer;	//���㻺���� VBO
	public:
		Line();
		Line(const Line&) = default;
		Line(const glm::vec3& p0, const glm::vec3& p1, const glm::vec4& color, int objectID = -1);
		~Line();

		glm::vec4& GetColor() { return m_Color; }
		void SetColor(const glm::vec4 & color) { m_Color = color; }

		LineVertex* GetVertices() { return m_Vertices; }
		void SetVertices(const LineVertex& v0, const LineVertex& v1) { m_Vertices[0] = v0;  m_Vertices[0] = v1; }

		float GetWidth() const { return m_Width; }
		void SetWidth(float width) { m_Width = width; }

		std::vector<LineVertex>& GetVertexBufferData() { return m_VertexBufferData; }

		const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
	};
}