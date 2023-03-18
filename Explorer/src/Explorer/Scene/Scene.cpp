#include "exppch.h"
#include "Scene.h"

#include "Explorer/Renderer/Renderer2D.h"
#include "Explorer/Renderer/Renderer3D.h"

#include "Components.h"

#include "Object.h"

namespace Explorer
{
	Scene::Scene(const std::string& name) :m_Name(name)
	{

	}

	Scene::~Scene()
	{

	}

	Object Scene::CreateObject(const std::string& name)
	{
		Object object = { m_Registry.create(), this };	//创建物体
		object.AddComponent<Transform>();				//添加Transform组件（默认组件）
		Name& objName = object.AddComponent<Name>();	//添加Name组件（默认组件）
		objName.m_Name = name;

		return object;
	}

	void Scene::DestroyEntity(Object object)
	{
		m_Registry.destroy(object);	//从注册表移除物体
	}

	void Scene::OnUpdateEditor(DeltaTime dt, EditorCamera& camera)
	{
		Renderer3D::BeginScene(camera);	//开始渲染场景

		auto group = m_Registry.group<Transform>(entt::get<SpriteRenderer>);	//返回有Transform和SpriteRenderer的所有物体

		for (auto object : group) {
			auto [transform, sprite] = group.get<Transform, SpriteRenderer>(object);

			Renderer3D::DrawMesh(transform, (int)object);	//绘制网格
		}
		Renderer3D::EndScene();			//结束渲染场景
	}

	void Scene::OnUpdate(DeltaTime dt)
	{
		//遍历所有拥有脚本组件的对象，调用each内的函数
		m_Registry.view<NativeScript>().each([=](auto object, NativeScript& script)
		{
			//脚本未实例化
			if (!script.Instance) {
				script.Instance = script.InstantiateScript();		//实例化脚本
				script.Instance->m_Object = Object{ object, this };	//设置脚本所属的对象

				script.Instance->OnCreate();	//调用脚本的OnCreate函数
			}
			script.Instance->OnUpdate(dt);		//调用脚本的OnOpdate函数
		});

		Camera* mainCamera = nullptr;	//主相机
		Transform* cameraTransform = nullptr;

		auto view = m_Registry.view<Transform, Camera>();	//返回有Transform和Camera的所有物体

		for (auto object : view) {
			auto [transform, camera] = view.get<Transform, Camera>(object);

			//找到主相机
			if (camera.m_Primary) {
				mainCamera = &camera;
				cameraTransform = &transform;
				break;
			}
		}

		//主相机存在
		if (mainCamera) {
			auto group = m_Registry.group<Transform>(entt::get<SpriteRenderer>);	//返回有Transform和SpriteRenderer的所有物体

			Renderer3D::BeginScene(*mainCamera, *cameraTransform);	//开始渲染场景
			for (auto object : group) {
				auto [transform, sprite] = group.get<Transform, SpriteRenderer>(object);

				//Renderer2D::DrawQuad(transform, sprite.m_Color);
				//Renderer3D::DrawMesh(transform);	//绘制网格
			}
			Renderer3D::EndScene();			//结束渲染场景
		}
	}
	
	void Scene::OnViewportResize(uint32_t width, uint32_t height)
	{
		m_ViewportWidth = width;
		m_ViewportHeight = height;

		auto view = m_Registry.view<Camera>();	//所有有Camera组件的物体

		for (auto object : view) {
			auto& camera = view.get<Camera>(object);	//获得Camera组件
			camera.SetViewportSize(width, height);		//设置视口大小
		}
	}

	Object Scene::GetPrimaryCameraObject()
	{
		auto view = m_Registry.view<Camera>();	//返回有Camera组件的物体
		for (auto object : view) {
			const auto& camera = view.get<Camera>(object);	//Camera组件
			//主相机
			if (camera.m_Primary) {
				return Object{ object, this };	//相机实体
			}
		}

		return {};
	}

	template<typename T>
	void Scene::OnComponentAdded(Object object, T& component)
	{
		static_assert(false);
	}

	template<>
	void Scene::OnComponentAdded<Transform>(Object object, Transform& component)
	{

	}

	/// <summary>
	/// Camera组件被添加时
	/// </summary>
	/// <param name="object">物体</param>
	/// <param name="component">Camera组件</param>
	template<>
	void Scene::OnComponentAdded<Camera>(Object object, Camera& component)
	{
		component.SetViewportSize(m_ViewportWidth, m_ViewportHeight);	//设置视口
	}

	template<>
	void Scene::OnComponentAdded<SpriteRenderer>(Object object, SpriteRenderer& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<Name>(Object object, Name& component)
	{

	}

	template<>
	void Scene::OnComponentAdded<NativeScript>(Object object, NativeScript& component)
	{

	}
}