#pragma once

#include "Texture.h"

#include "EditorCamera.h"
#include "Explorer/Components/Camera.h"
#include "Explorer/Components/Transform.h"

#include "Shader.h"
#include "VertexArray.h"

#include <glm/glm.hpp>

namespace Explorer
{
	/// <summary>
	/// ����
	/// </summary>
	struct Sphere
	{
		glm::vec3 Position{ 0.0f };
		float Radius = 0.5f;

		glm::vec3 Albedo{ 1.0f };
	};

	struct RayTracingScene
	{
		std::vector<Sphere> Spheres;
	};

	/// <summary>
	/// ����
	/// </summary>
	struct Ray
	{
		glm::vec3 Origin;		//Դ
		glm::vec3 Direction;	//����
	};

	/// <summary>
	/// ����׷����Ⱦ��
	/// </summary>
	class RendererRayTracing
	{
	private:
		std::shared_ptr<Texture2D> m_FinalImage;	//��Ⱦ���ͼ��
		uint32_t* m_ImageData = nullptr;			//ͼ������
	public:
		/// <summary>
		/// ��ʼ����Ⱦ��
		/// </summary>
		static void Init();

		static void Shutdown();

		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const Camera& camera, Transform& transform, const glm::vec2& viewportSize);

		static void Render(const RayTracingScene& scene);

		/// <summary>
		/// ��Ⱦͼ����Ⱦÿ������
		/// </summary>
		/// <param name="camera">���</param>
		//void Render(const EditorCamera& camera);

		std::shared_ptr<Texture2D> GetFinalImage() const { return m_FinalImage; }
	private:
		/// <summary>
		/// ׷�����ߣ��������������彻�㼰����ɫ
		/// </summary>
		/// <param name="ray">����</param>
		/// <returns>�����ɫ</returns>
		glm::vec4 TraceRay(const Ray& ray);
	};
}