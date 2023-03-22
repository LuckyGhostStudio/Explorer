#pragma once

#include "entt.hpp"

#include "Explorer/Core/DeltaTime.h"
#include "Explorer/Renderer/EditorCamera.h"
#include "Explorer/Components/Light.h"

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

		/// <summary>
		/// 创建空物体
		/// </summary>
		/// <param name="name">物体名</param>
		/// <returns>物体</returns>
		Object CreateEmptyObject(const std::string& name = "Object");
		
		/// <summary>
		/// 创建Camera
		/// </summary>
		/// <param name="name">相机名</param>
		/// <returns>相机</returns>
		Object CreateCameraObject(const std::string& name = "Camera");

		/// <summary>
		/// 创建Light
		/// </summary>
		/// <param name="type">光源类型</param>
		/// <param name="name">光源名</param>
		/// <returns>光源</returns>
		Object CreateLightObject(Light::Type type = Light::Type::Point, const std::string& name = "Light");

		/// <summary>
		/// 销毁物体
		/// </summary>
		/// <param name="object">物体</param>
		void DestroyObject(Object object);

		/// <summary>
		/// 编辑器更新：每帧调用
		/// </summary>
		/// <param name="dt">帧间隔</param>
		/// <param name="camera">编辑器相机</param>
		void OnUpdateEditor(DeltaTime dt, EditorCamera& camera);

		/// <summary>
		/// 更新：每帧调用
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
	};
}