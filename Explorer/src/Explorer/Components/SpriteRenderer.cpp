#include "exppch.h"
#include "SpriteRenderer.h"

namespace Explorer
{
	SpriteRenderer::SpriteRenderer()
	{
		Vertex vertices[] =
		{
			//--------λ��---------  -----------��ɫ------------  --------����--------  ---��������---  ID objID
			{{ -0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 0.0f }, 0, 0 },	//���� 0
			{{  0.5f, -0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 0.0f }, 1, 0 },	//���� 1
			{{  0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 1.0f, 1.0f }, 2, 0 },	//���� 2
			{{ -0.5f,  0.5f, 0.0f }, { 1.0f, 1.0f, 1.0f, 1.0f }, { 0.0f, 0.0f, 1.0f }, { 0.0f, 1.0f }, 3, 0 }	//���� 3
		};

		uint32_t indices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		m_Sprite = Sprite(vertices, indices);
	}
}