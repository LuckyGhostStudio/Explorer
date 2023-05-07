#pragma once

#include "Explorer/Core/DeltaTime.h"
#include "Explorer/Renderer/EditorCamera.h"
#include "Explorer/Components/Light.h"
#include "Explorer/Components/Mesh.h"
#include "Explorer/Components/Material.h"

#include "Explorer/Renderer/Shader.h"
#include "Explorer/Renderer/Environment.h"

#include "entt.hpp"

#include "Explorer/Core/UUID.h"

#include "Explorer/Renderer/RendererRayTracing.h"

#include <glm/glm.hpp>

class b2World;

namespace Explorer
{
	class Object;

	/// <summary>
	/// 场景
	/// </summary>
	class Scene
	{
	private:
		friend class Object;	//友元类Object
		friend class SceneHierarchyPanel;
		friend class SceneSerializer;

		entt::registry m_Registry;	//实体注册表：实体id集合（unsigned int集合）
		std::string m_Name;			//场景名

		uint32_t m_ViewportWidth = 1280;	//场景视口宽
		uint32_t m_ViewportHeight = 720;	//场景视口高

		Environment m_Environment;	//环境设置

		b2World* m_PhysicsWorld = nullptr;		//物理世界
		glm::vec2 m_Gravity = { 0.0f, -9.8f };	//重力加速度
	public:
		RayTracing::Scene m_RayTracingScene;	//TODO 待移除
	private:
		/// <summary>
		/// object添加T组件时调用
		/// </summary>
		/// <typeparam name="T">组件类型</typeparam>
		/// <param name="object">物体</param>
		/// <param name="component">组件</param>
		template<typename T>
		void OnComponentAdded(Object object, T& component);
	public:
		Scene(const std::string& name = "New Scene");
		~Scene();

		inline const std::string& GetName() const { return m_Name; }
		inline void SetName(const std::string& name) { m_Name = name; }

		Environment& GetEnvironment() { return m_Environment; }
		void SetEnvironment(const Environment& environment) { m_Environment = environment; }

		glm::vec2& GetGravity() { return m_Gravity; }
		void SetGravity(const glm::vec2& gravity) { m_Gravity = gravity; }

		/// <summary>
		/// 复制物体
		/// </summary>
		/// <param name="object">源物体</param>
		void CopyObject(Object object);

		/// <summary>
		/// 复制场景
		/// </summary>
		/// <param name="scene">待复制场景</param>
		/// <returns>复制体</returns>
		static std::shared_ptr<Scene> Copy(std::shared_ptr<Scene> scene);

		/// <summary>
		/// 创建空物体
		/// </summary>
		/// <param name="name">物体名</param>
		/// <param name="enable">物体启用状态</param>
		/// <returns>物体</returns>
		Object CreateEmptyObject(const std::string& name = "Object", bool enable = true);

		/// <summary>
		/// 创建空物体：使用已有UUID
		/// </summary>
		/// <param name="uuid">ID</param>
		/// <param name="name">物体名</param>
		/// <param name="enable">物体启用状态</param>
		/// <returns>物体</returns>
		Object CreateEmptyObject(UUID uuid, const std::string& name = "Object", bool enable = true);
		
		/// <summary>
		/// 创建图片物体
		/// </summary>
		/// <param name="name">物体名</param>
		/// <returns>物体</returns>
		Object CreateSpriteObject(const std::string& name = "Sprite");

		/// <summary>
		/// 创建Circle
		/// </summary>
		/// <param name="name">物体名</param>
		/// <returns>物体</returns>
		Object CreateCircleObject(const std::string& name = "Circle");

		/// <summary>
		/// 创建网格物体
		/// </summary>
		/// <param name="name">物体名</param>
		/// <param name="type">物体网格类型</param>
		/// <returns>网格物体</returns>
		Object CreateMeshObject(const std::string& name = "Object", const Mesh::Type type = Mesh::Type::None);

		/// <summary>
		/// 创建Camera
		/// </summary>
		/// <param name="name">相机名</param>
		/// <param name="primary">是否是主相机</param>
		/// <returns>相机</returns>
		Object CreateCameraObject(const std::string& name = "Camera", bool primary = false);

		/// <summary>
		/// 创建Light
		/// </summary>
		/// <param name="type">光源类型</param>
		/// <param name="name">光源名</param>
		/// <returns>光源</returns>
		Object CreateLightObject(Light::Type type = Light::Type::Point, const std::string& name = "Light");

		/// <summary>
		/// 创建光线追踪测试球体
		/// </summary>
		/// <param name="name">物体名</param>
		/// <returns>球体</returns>
		Object CreateSphereObjectToRayTracing(const std::string& name = "Sphere");

		/// <summary>
		/// 销毁物体
		/// </summary>
		/// <param name="object">物体</param>
		void DestroyObject(Object object);

		/// <summary>
		/// 开始运行时调用：点击Play时（开始物理模拟）
		/// </summary>
		void OnRuntimeStart();

		/// <summary>
		/// 停止运行时调用：停止Play时（停止物理模拟）
		/// </summary>
		void OnRuntimeStop();

		/// <summary>
		/// 编辑器更新：每帧调用
		/// </summary>
		/// <param name="dt">帧间隔</param>
		/// <param name="camera">编辑器相机</param>
		void OnUpdateEditor(DeltaTime dt, EditorCamera& camera);

		/// <summary>
		/// 运行时更新：每帧调用
		/// </summary>
		/// <param name="dt">帧间隔</param>
		void OnUpdate(DeltaTime dt);

		/// <summary>
		/// 重置视口大小：视口改变时调用
		/// </summary>
		/// <param name="width">宽</param>
		/// <param name="height">高</param>
		void OnViewportResize(uint32_t width, uint32_t height);

		/// <summary>
		/// 返回主相机
		/// </summary>
		/// <returns>主相机</returns>
		Object GetPrimaryCameraObject();

		/// <summary>
		/// 返回拥有Components组件的所有物体
		/// </summary>
		/// <typeparam name="...Components">组件类型列表</typeparam>
		/// <returns>物体列表</returns>
		template<typename... Components>
		auto GetAllObjectsWith()
		{
			return m_Registry.view<Components...>();
		}
	};
}