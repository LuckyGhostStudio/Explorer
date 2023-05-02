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
	/// 球体
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
	class RendererRayTracing
	{
	private:
		std::shared_ptr<Texture2D> m_FinalImage;	//渲染结果图像
		uint32_t* m_ImageData = nullptr;			//图像数据
	public:
		/// <summary>
		/// 初始化渲染器
		/// </summary>
		static void Init();

		static void Shutdown();

		static void BeginScene(const EditorCamera& camera);
		static void BeginScene(const Camera& camera, Transform& transform, const glm::vec2& viewportSize);

		static void Render(const RayTracingScene& scene);

		/// <summary>
		/// 渲染图像：渲染每个像素
		/// </summary>
		/// <param name="camera">相机</param>
		//void Render(const EditorCamera& camera);

		std::shared_ptr<Texture2D> GetFinalImage() const { return m_FinalImage; }
	private:
		/// <summary>
		/// 追踪射线：计算射线与物体交点及其颜色
		/// </summary>
		/// <param name="ray">射线</param>
		/// <returns>输出颜色</returns>
		glm::vec4 TraceRay(const Ray& ray);
	};
}