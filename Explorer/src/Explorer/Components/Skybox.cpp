#include "exppch.h"
#include "Skybox.h"

#include "Explorer/Renderer/RenderCommand.h"
#include "SubMesh.h"

#include <glad/glad.h>

namespace Explorer
{
	Skybox::Skybox()
	{
		SetCubeVertexBuffer();	//���ö��㻺����

		m_Shader = ShaderLibrary::Get("Skybox");	//Skybox��ɫ��
		m_Shader->Bind();

		m_Cubemap = std::make_shared<Cubemap>();	//����Cubemap
		m_Cubemap->Bind();							//��Cubemap

		//����Ĭ��Ԥ������
		for (auto& texture : m_PreviewTextures) {
			texture = std::make_shared<Texture2D>(1, 1);			//�������Ϊ1����������ȱʧʱ��Ԥ������
			uint32_t loseTextureData = 0x0eeeeeff;					//��ɫ
			texture->SetData(&loseTextureData, sizeof(uint32_t));	//������������size = 1 * 1 * 4 == sizeof(uint32_t)
		}
	}

	void Skybox::SetCubeVertexBuffer()
	{
		std::vector<float> vertices =
		{
			//--------λ��------D
			 1.0f, -1.0f,  1.0f,	// A 0 x+
			 1.0f, -1.0f,  1.0f,	// A 1 y-
			 1.0f, -1.0f,  1.0f,	// A 2 z+

			 1.0f, -1.0f, -1.0f,	// B 3
			 1.0f, -1.0f, -1.0f,	// B 4
			 1.0f, -1.0f, -1.0f,	// B 5

			 1.0f,  1.0f, -1.0f,	// C 6
			 1.0f,  1.0f, -1.0f,	// C 7
			 1.0f,  1.0f, -1.0f,	// C 8

			 1.0f,  1.0f,  1.0f,	// D 9
			 1.0f,  1.0f,  1.0f,	// D 10
			 1.0f,  1.0f,  1.0f,	// D 11

			-1.0f, -1.0f,  1.0f,	// E 12
			-1.0f, -1.0f,  1.0f,	// E 13
			-1.0f, -1.0f,  1.0f,	// E 14
	
			-1.0f, -1.0f, -1.0f,	// F 15
			-1.0f, -1.0f, -1.0f,	// F 16
			-1.0f, -1.0f, -1.0f,	// F 17
	
			-1.0f,  1.0f, -1.0f,	// G 18
			-1.0f,  1.0f, -1.0f,	// G 19
			-1.0f,  1.0f, -1.0f,	// G 20
			
			-1.0f,  1.0f,  1.0f,	// H 21
			-1.0f,  1.0f,  1.0f,	// H 22
			-1.0f,  1.0f,  1.0f		// H 23
		};

		//��������
		std::vector<uint32_t> indices =
		{
			0, 3, 6,	// A B C x+
			6, 9, 0,	// C D A x+
			18, 15, 12,	// G F E x-
			18, 21, 12,	// G H E x-
			22, 7, 19,	// H C G y+
			7, 10, 22,	// C D H y+
			13, 16, 4,	// E F B y-
			4, 1, 13,	// B A E y-
			23, 14, 2,	// H E A z+
			2, 11, 23,	// A D H z+
			20, 5, 17,	// G B F z-
			5, 20, 8,	// B G C z-
		};

		m_VertexArray = std::make_shared<VertexArray>();								//������������VAO
		m_VertexBuffer = std::make_shared<VertexBuffer>(vertices.data(), vertices.size() * sizeof(float));	//�������㻺��VBO

		//���ö��㻺��������
		m_VertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//λ��
		});
		m_VertexArray->AddVertexBuffer(m_VertexBuffer);	//���VBO��VAO

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(indices.data(), indices.size());	//������������EBO
		m_VertexArray->SetIndexBuffer(indexBuffer);												//����EBO��VAO
	}
	
	void Skybox::SetCubemapOneSideTexture(const std::string& filepath, Cubemap::TextureDirection direction)
	{
		int index = (int)direction;				//��ͼ����
		m_PreviewTextures[index] = std::make_shared<Texture2D>(filepath); //����Ԥ��������ͨ����
		m_PreviewTextures[index]->Bind(index);	//��

		//����Up��Down��ͼ��Cubemap����y��ߵ�
		if (direction == Cubemap::TextureDirection::Up) direction = Cubemap::TextureDirection::Down;
		else if (direction == Cubemap::TextureDirection::Down) direction = Cubemap::TextureDirection::Up;

		m_Cubemap->SetSide(filepath, direction);	//����Cubemap��direction����ͼ
		m_Cubemap->Bind();							//��Cubemap��
	}
}