#pragma once

#include "Explorer/Renderer/Texture.h"
#include "Explorer/Renderer/VertexArray.h"

#include "Vertex.h"

#include <glm/glm.hpp>

namespace Explorer
{
	class Sprite
	{
	private:
		Vertex m_Vertices[4];
		uint32_t m_VertexIndices[6];
		std::shared_ptr<Texture2D> m_Texture;

		std::vector<Vertex> m_VertexBufferData;			//���㻺�������ݣ�����Ҫ��Ⱦ�Ķ�������

		std::shared_ptr<VertexArray> m_VertexArray;		//�������� VAO
		std::shared_ptr<VertexBuffer> m_VertexBuffer;	//���㻺���� VBO
	public:
		Sprite() = default;
		Sprite(const Sprite&) = default;
		Sprite(const Vertex* vertices, const uint32_t* indices);
		~Sprite();

		Vertex* GetVertices() { return m_Vertices; }
		void SetVertices(const Vertex* vertices) { for (int i = 0; i < 4; i++) m_Vertices[i] = vertices[i]; }

		uint32_t* GetVertexIndices() { return m_VertexIndices; }
		void SetVertices(const uint32_t* indices) { for (int i = 0; i < 6; i++) m_VertexIndices[i] = indices[i]; }

		std::shared_ptr<Texture2D>& GetTexture() { return m_Texture; }
		void SetTexture(const std::shared_ptr<Texture2D>& texture) { m_Texture = texture; }

		std::vector<Vertex>& GetVertexBufferData() { return m_VertexBufferData; }

		const std::shared_ptr<VertexArray>& GetVertexArray() const { return m_VertexArray; }
		const std::shared_ptr<VertexBuffer>& GetVertexBuffer() const { return m_VertexBuffer; }
	};
}