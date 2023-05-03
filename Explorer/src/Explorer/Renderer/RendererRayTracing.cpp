#include "exppch.h"
#include "RendererRayTracing.h"

#include "Explorer/Utils/Random.h"

#include "RenderCommand.h"

namespace Explorer
{
	namespace Utils
	{
		/// <summary>
		/// vec4ת��Ϊ32λRBGA��16���ƣ�
		/// </summary>
		/// <param name="color">��ɫ</param>
		/// <returns>32λ16������ɫ</returns>
		static uint32_t ConvertToRGBA(const glm::vec4& color)
		{
			uint8_t r = (uint8_t)(color.r * 255.0f);	//��ɫͨ��
			uint8_t g = (uint8_t)(color.g * 255.0f);	//��ɫͨ��
			uint8_t b = (uint8_t)(color.b * 255.0f);	//��ɫͨ��
			uint8_t a = (uint8_t)(color.a * 255.0f);	//͸��ͨ��

			uint32_t result = (a << 24) | (b << 16) | (g << 8) | r;	//ABGR

			return result;
		}
	}

	/// <summary>
	/// ��Ⱦ������
	/// </summary>
	struct RendererData
	{
		std::shared_ptr<Shader> Shader;
		std::shared_ptr<VertexArray> FinalRectVertexArray;		//��Ⱦ�����������VAO
		std::shared_ptr<VertexBuffer> FinalRectVertexBuffer;	//��Ⱦ�����������VBO
	};

	static RendererData s_Data;	//��Ⱦ����

	void RendererRayTracing::Init()
	{
		s_Data.Shader = std::make_shared<Shader>("assets/shaders/RayTracingShader");	//����׷����ɫ��

		float finalRectVertexPositions[12] =
		{
			-1.0f, -1.0f, 0.0f,	//���� 0
			 1.0f, -1.0f, 0.0f,	//���� 1
			 1.0f,  1.0f, 0.0f,	//���� 2
			-1.0f,  1.0f, 0.0f	//���� 3
		};

		uint32_t finalRectVertexIndices[] =
		{
			0, 1, 2,
			2, 3, 0
		};

		s_Data.FinalRectVertexArray = std::make_shared<VertexArray>();				//������������VAO
		s_Data.FinalRectVertexBuffer = std::make_shared<VertexBuffer>(finalRectVertexPositions, 12 * sizeof(float));	//�������㻺��VBO

		//���ö��㻺��������
		s_Data.FinalRectVertexBuffer->SetLayout({
			{ShaderDataType::Float3, "a_Position"},	//λ��
		});
		s_Data.FinalRectVertexArray->AddVertexBuffer(s_Data.FinalRectVertexBuffer);	//���VBO��VAO

		std::shared_ptr<IndexBuffer> indexBuffer = std::make_shared<IndexBuffer>(finalRectVertexIndices, 6);	//������������EBO
		s_Data.FinalRectVertexArray->SetIndexBuffer(indexBuffer);												//����EBO��VAO
	}

	void RendererRayTracing::Shutdown()
	{

	}

	void RendererRayTracing::BeginScene(const EditorCamera& camera)
	{
		s_Data.Shader->Bind();

		s_Data.Shader->SetFloat3("u_Camera.Position", camera.GetPosition());
		s_Data.Shader->SetFloat2("u_Camera.ViewportSize", camera.GetViewportSize());
		s_Data.Shader->SetMat4("u_Camera.ProjectionMatrix", camera.GetProjectionMatrix());
		s_Data.Shader->SetMat4("u_Camera.ViewMatrix", camera.GetViewMatrix());
	}

	void RendererRayTracing::BeginScene(const Camera& camera, Transform& transform, const glm::vec2& viewportSize)
	{
		s_Data.Shader->Bind();

		s_Data.Shader->SetFloat3("u_Camera.Position", transform.GetPosition());
		s_Data.Shader->SetFloat2("u_Camera.ViewportSize", viewportSize);
		s_Data.Shader->SetMat4("u_Camera.ProjectionMatrix", camera.GetProjection());
		s_Data.Shader->SetMat4("u_Camera.ViewMatrix", glm::inverse(transform.GetTransform()));
	}

	void RendererRayTracing::Render(const RayTracingScene& scene)
	{
		s_Data.Shader->Bind();

		//���ó�������uniform����
		s_Data.Shader->SetInt("u_Scene.ObjectCount", scene.Spheres.size());
		s_Data.Shader->SetInt("u_Scene.MaterialCount", scene.Materials.size());

		for (int i = 0; i < scene.Spheres.size(); i++) {
			s_Data.Shader->SetFloat3("u_Scene.Spheres[" + std::to_string(i) + "].Center", scene.Spheres[i].Position);
			s_Data.Shader->SetFloat("u_Scene.Spheres[" + std::to_string(i) + "].Radius", scene.Spheres[i].Radius);
			s_Data.Shader->SetInt("u_Scene.Spheres[" + std::to_string(i) + "].MaterialIndex", scene.Spheres[i].MaterialIndex);
		}

		for (int i = 0; i < scene.Materials.size(); i++) {
			s_Data.Shader->SetFloat3("u_Scene.Materials[" + std::to_string(i) + "].Albedo", scene.Materials[i].Albedo);
			s_Data.Shader->SetFloat("u_Scene.Materials[" + std::to_string(i) + "].Roughness", scene.Materials[i].Roughness);
			s_Data.Shader->SetFloat("u_Scene.Materials[" + std::to_string(i) + "].Metallic", scene.Materials[i].Metallic);
		}

		RenderCommand::DrawIndexed(s_Data.FinalRectVertexArray);	//������Ⱦ�������
	}

#if 0
	void RendererRayTracing::Render(const EditorCamera& camera)
	{
		Ray ray;
		//ray.Origin = camera.GetPosition();	//����Դ
		ray.Origin = { 0, 0, 1 };

		//����ÿ������
		for (uint32_t y = 0; y < m_FinalImage->GetHeight(); y++){
			for (uint32_t x = 0; x < m_FinalImage->GetWidth(); x++){
				glm::vec2 coord = { (float)x / (float)m_FinalImage->GetWidth(), (float)y / (float)m_FinalImage->GetHeight() };	//����[0, 1]
				coord = coord * 2.0f - 1.0f;	//coord = [-1, 1] �������Ļ����(0, 0)λ��
				//coord.x *= aspectRatio;

				//ray.Direction = camera.GetRayDirections()[x + y * m_FinalImage->GetWidth()];	//���߷���
				ray.Direction = { coord.x, coord.y, -1.0f };

				glm::vec4 color = TraceRay(ray);	//coord�����������ɫ
				color = glm::clamp(color, glm::vec4(0.0f), glm::vec4(1.0f));	//����color = [0, 1]
				m_ImageData[x + y * m_FinalImage->GetWidth()] = Utils::ConvertToRGBA(color);	//ͼ������
			}
		}
		
		m_FinalImage->SetData(m_ImageData, m_FinalImage->GetWidth() * m_FinalImage->GetHeight() * sizeof(uint32_t));
	}
#endif

	glm::vec4 RendererRayTracing::TraceRay(const Ray& ray)
	{
		float radius = 0.5f;								//��뾶

		float a = glm::dot(ray.Direction, ray.Direction);				//������ϵ����direction.x ^ 2 + direction.y ^ 2 + direction.z ^ 2
		float b = 2.0f * glm::dot(ray.Origin, ray.Direction);			//һ����ϵ����2 * (origin.x * direction.x + origin.y * direction.y + origin.z * direction.z)
		float c = glm::dot(ray.Origin, ray.Origin) - radius * radius;	//�����origin.x ^ 2 + origin.y ^ 2 + origin.z ^ 2 - radius ^ 2

		float discriminant = b * b - 4.0f * a * c;		//�������򽻵��б�ʽ

		//�޽���
		if (discriminant < 0.0f) {
			return glm::vec4(0.1, 0.1, 0.1, 1);
		}

		float t0 = (-b + glm::sqrt(discriminant)) / 2.0f * a;		//Դ���뽻��0����
		float closetT = (-b - glm::sqrt(discriminant)) / 2.0f * a;	//Դ�����������ľ���

		glm::vec3 hitPoint = ray.Origin + ray.Direction * closetT;	//�����������
		glm::vec3 normal = glm::normalize(hitPoint);				//���㷨��

		glm::vec3 lightDir = glm::normalize(glm::vec3(-1, -1, -1));	//���շ���
		float d = glm::max(glm::dot(normal, -lightDir), 0.0f);		//����ǿ��[0, 1]

		glm::vec3 sphereColor(1, 0, 1);
		sphereColor *= d;
		return glm::vec4(sphereColor, 1);
	}
}