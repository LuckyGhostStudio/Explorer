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
	/// 基于物理的材质：金属/非金属
	/// </summary>
	struct PBRMaterial
	{
		glm::vec3 Albedo{ 1.0f };

		float Roughness = 1.0f;	//粗糙度
	};

	/// <summary>
	/// 球体
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
	/// 射线
	/// </summary>
	struct Ray
	{
		glm::vec3 Origin;		//源
		glm::vec3 Direction;	//方向
	};

	/// <summary>
	/// 光线追踪渲染器
	/// </summary>
	class Renderer
	{
	public:
		/// <summary>
		/// 初始化渲染器
		/// </summary>
		static void Init();

		static void Shutdown();

		static void BeginScene(const Explorer::EditorCamera& camera);
		static void BeginScene(const Explorer::Camera& camera, Explorer::Transform& transform, const glm::vec2& viewportSize);

		static void Render(const Scene& scene);
	};
}