#pragma once

#include "EditorCamera.h"
#include "Explorer/Components/Camera.h"
#include "Explorer/Components/Transform.h"

namespace Explorer
{
	class Object;
}

namespace RayTracing
{
	/// <summary>
	/// ��������Ĳ��ʣ�����/�ǽ���
	/// </summary>
	struct PBRMaterial
	{
		glm::vec3 Albedo{ 1.0f };

		float Roughness = 1.0f;	//�ֲڶ�
	};

	/// <summary>
	/// ����
	/// </summary>
	struct Sphere
	{
		glm::vec3 Position{ 0.0f };
		float Radius = 0.5f;

		int MaterialIndex = 0;
	};

	struct Scene
	{
		std::vector<Explorer::Object> SphereObjects;
		std::vector<Sphere> Spheres;
		std::vector<PBRMaterial> Materials;
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
	class Renderer
	{
	public:
		/// <summary>
		/// ��ʼ����Ⱦ��
		/// </summary>
		static void Init();

		static void Shutdown();

		static void BeginScene(const Explorer::EditorCamera& camera);
		static void BeginScene(const Explorer::Camera& camera, Explorer::Transform& transform, const glm::vec2& viewportSize);

		static void Render(const Scene& scene);
	};
}